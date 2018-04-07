import gc
import gym
import gzip
import gym.spaces
import json
import numpy as np
import os
import retro
from gym.utils import seeding
from retro.data import GameData

gym_version = tuple(int(x) for x in gym.__version__.split('.'))


class RetroEnv(gym.Env):
    metadata = {'render.modes': ['human', 'rgb_array'],
                'video.frames_per_second': 60.0}

    def compute_step(self, image):
        reward = self.data.current_reward()
        done = self.data.is_done()
        return reward, done, self.data.lookup_all()

    def record_movie(self, path):
        self.movie = retro.Movie(path, True)
        self.movie.configure(self.gamename, self.em)
        if self.initial_state:
            self.movie.set_state(self.initial_state)

    def stop_record(self):
        self.movie_path = None
        self.movie_id = 0
        if self.movie:
            self.movie.close()
            self.movie = None

    def auto_record(self, path=None):
        if not path:
            path = os.getcwd()
        self.movie_path = path

    def __init__(self, game, state=retro.STATE_DEFAULT, scenario=None, info=None, use_restricted_actions=retro.ACTIONS_FILTERED, record=False):
        if not hasattr(self, 'spec'):
            self.spec = None
        self.img = None
        self.viewer = None
        self.gamename = game
        self.statename = state

        game_path = retro.get_game_path(game)
        rom_path = retro.get_romfile_path(game)
        metadata_path = os.path.join(game_path, 'metadata.json')

        if state == retro.STATE_NONE:
            self.initial_state = None
        elif state == retro.STATE_DEFAULT:
            self.initial_state = None
            try:
                with open(metadata_path) as f:
                    metadata = json.load(f)
                if 'default_state' in metadata:
                    with gzip.open(os.path.join(game_path, metadata['default_state']) + '.state', 'rb') as fh:
                        self.initial_state = fh.read()
            except (IOError, json.JSONDecodeError):
                pass
        else:
            if not state.endswith('.state'):
                state += '.state'

            with gzip.open(os.path.join(game_path, state), 'rb') as fh:
                self.initial_state = fh.read()

        self.data = GameData()

        if info is None:
            info = 'data'

        if info.endswith('.json'):
            # assume it's a path
            info_path = info
        else:
            info_path = os.path.join(game_path, info + '.json')

        if scenario is None:
            scenario = 'scenario'

        if scenario.endswith('.json'):
            # assume it's a path
            scenario_path = scenario
        else:
            scenario_path = os.path.join(game_path, scenario + '.json')

        system = retro.get_romfile_system(rom_path)

        # We can't have more than one emulator per process. Before creating an
        # emulator, ensure that unused ones are garbage-collected
        gc.collect()
        self.em = retro.RetroEmulator(rom_path)
        self.em.configure_data(self.data)
        self.em.step()
        img = self.em.get_screen()

        core = retro.get_system_info(system)
        self.BUTTONS = core['buttons']
        self.NUM_BUTTONS = len(self.BUTTONS)
        self.BUTTON_COMBOS = self.data.valid_actions()

        try:
            assert self.data.load(info_path, scenario_path), 'Failed to load info (%s) or scenario (%s)' % (info_path, scenario_path)
        except Exception:
            del self.em
            raise

        if use_restricted_actions == retro.ACTIONS_DISCRETE:
            combos = 1
            for combo in self.BUTTON_COMBOS:
                combos *= len(combo)
            self.action_space = gym.spaces.Discrete(combos)
        elif use_restricted_actions == retro.ACTIONS_MULTI_DISCRETE:
            self.action_space = gym.spaces.MultiDiscrete([len(combos) if gym_version >= (0, 9, 6) else (0, len(combos) - 1) for combos in self.BUTTON_COMBOS])
        else:
            self.action_space = gym.spaces.MultiBinary(self.NUM_BUTTONS)

        kwargs = {}
        if gym_version >= (0, 9, 6):
            kwargs['dtype'] = np.uint8
        self.observation_space = gym.spaces.Box(low=0, high=255, shape=img.shape, **kwargs)

        self.use_restricted_actions = use_restricted_actions
        self.movie = None
        self.movie_id = 0
        self.movie_path = None
        if record is True:
            self.auto_record()
        elif record is not False:
            self.auto_record(record)
        self.seed()
        if gym_version < (0, 9, 6):
            self._seed = self.seed
            self._step = self.step
            self._reset = self.reset
            self._render = self.render
            self._close = self.close

    def step(self, a):
        action = 0
        if self.use_restricted_actions == retro.ACTIONS_DISCRETE:
            for combo in self.BUTTON_COMBOS:
                current = a % len(combo)
                a //= len(combo)
                action |= combo[current]
        elif self.use_restricted_actions == retro.ACTIONS_MULTI_DISCRETE:
            for i in range(len(a)):
                buttons = self.BUTTON_COMBOS[i]
                action |= buttons[a[i]]
        else:
            for i in range(len(a)):
                action |= int(a[i]) << i
            if self.use_restricted_actions == retro.ACTIONS_FILTERED:
                action = self.data.filter_action(action)
        a = np.zeros([16], np.uint8)
        for i in range(16):
            a[i] = (action >> i) & 1
            if self.movie:
                self.movie.set_key(i, a[i])
        if self.movie:
            self.movie.step()
        self.em.set_button_mask(a)
        self.em.step()
        self.img = ob = self.em.get_screen()
        self.data.update_ram()
        rew, done, info = self.compute_step(ob)
        return ob, float(rew), bool(done), dict(info)

    def reset(self):
        if self.initial_state:
            self.em.set_state(self.initial_state)
        self.em.set_button_mask(np.zeros([16], np.uint8))
        self.em.step()
        if self.movie_path is not None:
            self.record_movie(os.path.join(self.movie_path, '%s-%s-%04d.bk2' % (self.gamename, self.statename, self.movie_id)))
            self.movie_id += 1
        if self.movie:
            self.movie.step()
        self.img = ob = self.em.get_screen()
        self.data.reset()
        self.data.update_ram()
        return ob

    def seed(self, seed=None):
        self.np_random, seed1 = seeding.np_random(seed)
        # Derive a random seed. This gets passed as a uint, but gets
        # checked as an int elsewhere, so we need to keep it below
        # 2**31.
        seed2 = seeding.hash_seed(seed1 + 1) % 2**31
        return [seed1, seed2]

    def render(self, mode='human', close=False):
        if close:
            if self.viewer:
                self.viewer.close()
            return
        if mode == "rgb_array":
            return self.em.get_screen() if self.img is None else self.img
        elif mode == "human":
            if self.viewer is None:
                from gym.envs.classic_control.rendering import SimpleImageViewer
                self.viewer = SimpleImageViewer()
            self.viewer.imshow(self.img)
            return self.viewer.isopen

    def close(self):
        if hasattr(self, 'em'):
            del self.em

        if self.viewer is not None:
            self.viewer.close()
            self.viewer = None
