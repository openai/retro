"""
Example wrapper to improve determinism of Retro environments
"""

import retro
import numpy as np
import argparse
import gym
import multiprocessing as mp

CHUNK_LENGTH = 128


class MoreDeterministicRetroState(gym.Wrapper):
    """
    Save/restore state on each step to avoid de-sync
    It's likely that reward and done will not be correct if they
    depend on lua state (e.g. Sonic "contest" scenario)

    For most emulated systems this is 10%-50% slower, for Atari2600 it is
    60x slower.  It's unclear why stella is slow slow to save/load a state.
    This also fails on GameBoy games due to https://github.com/openai/retro/issues/116

    If other wrappers have state (such as Timelimit), they would need to be extended
    to support get_state() and reset(state=state), and then this class would need
    to make sure parent methods are called.
    """
    def __init__(self, *args, reset_on_step=True, **kwargs):
        super().__init__(*args, **kwargs)
        self._last_obs = None
        self._done = False
        # if retro were more deterministic, this would not be necessary
        self._reset_on_step = reset_on_step

    def reset(self, state=None):
        self._done = False
        if state is not None:
            em_state, self._last_obs = state
            self.unwrapped.em.set_state(em_state)
            self.unwrapped.data.reset()
            self.unwrapped.data.update_ram()
        else:
            self._last_obs = self.env.reset()
        return self._last_obs

    def step(self, act):
        if self._reset_on_step:
            self.reset(state=self.get_state())
        self._last_obs, rew, self._done, info = self.env.step(act)
        return self._last_obs, rew, self._done, info

    def get_state(self):
        assert not self._done, "cannot store a terminal state"
        return (self.unwrapped.em.get_state(), self._last_obs)


def rollout(env, acts):
    total_rew = 0.0
    for act in acts:
        _obs, rew, done, _info = env.step(act)
        total_rew += rew
        if done:
            break
    return total_rew

    
def chunk(L, length):
    result = []
    while True:
        sublist = L[:length]
        if len(sublist) == 0:
            break
        L = L[length:]
        result.append(sublist)
    return result

    
def partition(L, pieces):
    return chunk(L, len(L) // pieces + 1)


def check_env_helper(make_env, all_acts, verbose, out_success):
    # do rollouts and get reference values
    env = make_env()
    env.reset()

    # truncate actions to end before done
    valid_acts = []
    for act in all_acts:
        _obs, _rew, done, _info = env.step(act)
        if done:
            break
        valid_acts.append(act)

    env.reset()
    in_states = [env.get_state()]
    in_acts = chunk(valid_acts, CHUNK_LENGTH)
    out_rews = []
    out_rams = []

    for acts in in_acts:
        out_rews.append(rollout(env, acts))
        out_rams.append(env.get_ram())
        in_states.append(env.get_state())

    in_states.pop()  # remove extra final state since there are no actions after it

    success = True
    for start_idx in range(len(in_states)):
        if verbose:
            print(start_idx+1, len(in_states))
        env.reset(state=in_states[start_idx])

        for offset, acts in enumerate(in_acts[start_idx:]):
            if not np.array_equal(rollout(env, acts), out_rews[start_idx+offset]):
                print('failed rew')
                success = False
            if not np.array_equal(env.get_ram(), out_rams[start_idx+offset]):
                print('failed ram')
                success = False

    env.close()
    out_success.value = success


def check_env(make_env, acts, verbose=False, timeout=None):
    out_success = mp.Value('b', False)
    p = mp.Process(target=check_env_helper, args=(make_env, acts, verbose, out_success), daemon=True)
    p.start()
    p.join(timeout)
    if p.is_alive():
        print('failed to finish in time')
        p.terminate()
        p.join()
        return False
    return bool(out_success.value)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--deterministic', action='store_true', help='use deterministic wrapper')
    parser.add_argument('--suffix', default='', help='run against games matching this suffix')
    parser.add_argument('--movie-file', help='load a bk2 and use states obtained from replaying actions from the bk2')
    args = parser.parse_args()

    if args.movie_file is None:
        games = [g for g in sorted(retro.data.list_games()) if g.endswith(args.suffix)]

        failed_games = []
        for game in games:
            print(game)
            def make_env():
                env = retro.make(game=game)
                if args.deterministic:
                    env = MoreDeterministicRetroState(env)
                else:
                    env = MoreDeterministicRetroState(env, reset_on_step=False)
                return env

            env = make_env()
            env.action_space.seed(0)
            acts = [env.action_space.sample() for _ in range(CHUNK_LENGTH * 2)]
            env.close()

            if not check_env(make_env, acts, timeout=128):
                failed_games.append(game)
        
        for game in failed_games:
            print('failed:', game)

    elif args.movie_file is not None:
        movie = retro.Movie(args.movie_file)
        movie.step()

        def make_env():
            env = retro.make(movie.get_game(), state=retro.State.DEFAULT, use_restricted_actions=retro.Actions.ALL)
            env.initial_state = movie.get_state()
            if args.deterministic:
                env = MoreDeterministicRetroState(env)
            else:
                env = RetroState(env)
            return env

        env = make_env()
        acts = []
        while movie.step():
            act = []
            for p in range(movie.players):
                for i in range(env.num_buttons):
                    act.append(movie.get_key(i, p))
            acts.append(act)
        env.close()
        check_env(make_env, acts, verbose=True)
    else:
        raise Exception('must specify --suffix or --movie-file')


if __name__ == '__main__':
    main()