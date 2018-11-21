import gc
import gym
import gzip
import gym.spaces
import json
import numpy as np
import os
import retro
import retro.data
from gym.utils import seeding

from retro.data import list_games, list_states

gym_version = tuple(int(x) for x in gym.__version__.split('.'))

__all__ = ['RetroEnv']


class RetroEnv(gym.Env):
    metadata = {'render.modes': ['human', 'rgb_array'],
                'video.frames_per_second': 60.0}

    def compute_step(self):
        if self.players > 1:
            reward = [self.data.current_reward(p) for p in range(self.players)]
        else:
            reward = self.data.current_reward()
        done = self.data.is_done()
        return reward, done, self.data.lookup_all()

    def record_movie(self, path):
        self.movie = retro.Movie(path, True, self.players)
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

    def __init__(self, game, state=retro.State.DEFAULT, scenario=None, info=None, use_restricted_actions=retro.Actions.FILTERED, record=False, players=1, inttype=retro.data.Integrations.STABLE):
        if not hasattr(self, 'spec'):
            self.spec = None
        self.img = None
        self.viewer = None
        self.gamename = game
        self.statename = state
        self.initial_state = None
        self.players = players

        metadata = {}
        rom_path = retro.data.get_romfile_path(game, inttype)
        metadata_path = retro.data.get_file_path(game, 'metadata.json', inttype)

        if state == retro.State.NONE:
            self.statename = None
        elif state == retro.State.DEFAULT:
            self.statename = None
            try:
                with open(metadata_path) as f:
                    metadata = json.load(f)
                if 'default_player_state' in metadata and self.players <= len(metadata['default_player_state']):
                    self.statename = metadata['default_player_state'][self.players - 1]
                elif 'default_state' in metadata:
                    self.statename = metadata['default_state']
                else:
                    self.statename = None
            except (IOError, json.JSONDecodeError):
                pass

        if self.statename:
            self.load_state(self.statename, inttype)

        self.data = retro.data.GameData()

        if info is None:
            info = 'data'

        if info.endswith('.json'):
            # assume it's a path
            info_path = info
        else:
            info_path = retro.data.get_file_path(game, info + '.json', inttype)

        if scenario is None:
            scenario = 'scenario'

        if scenario.endswith('.json'):
            # assume it's a path
            scenario_path = scenario
        else:
            scenario_path = retro.data.get_file_path(game, scenario + '.json', inttype)

        self.system = retro.get_romfile_system(rom_path)

        # We can't have more than one emulator per process. Before creating an
        # emulator, ensure that unused ones are garbage-collected
        gc.collect()
        self.em = retro.RetroEmulator(rom_path)
        self.em.configure_data(self.data)
        self.em.step()

        core = retro.get_system_info(self.system)
        self.buttons = core['buttons']
        self.num_buttons = len(self.buttons)
        self.button_combos = self.data.valid_actions()

        try:
            assert self.data.load(info_path, scenario_path), 'Failed to load info (%s) or scenario (%s)' % (info_path, scenario_path)
        except Exception:
            del self.em
            raise

        img = [self.get_screen(p) for p in range(players)]

        if use_restricted_actions == retro.Actions.DISCRETE:
            combos = 1
            for combo in self.button_combos:
                combos *= len(combo)
            self.action_space = gym.spaces.Discrete(combos ** players)
        elif use_restricted_actions == retro.Actions.MULTI_DISCRETE:
            self.action_space = gym.spaces.MultiDiscrete([len(combos) if gym_version >= (0, 9, 6) else (0, len(combos) - 1) for combos in self.button_combos] * players)
        else:
            self.action_space = gym.spaces.MultiBinary(self.num_buttons * players)

        kwargs = {}
        if gym_version >= (0, 9, 6):
            kwargs['dtype'] = np.uint8
        self.observation_space = gym.spaces.Box(low=0, high=255, shape=img[0].shape, **kwargs)

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

    def action_to_array(self, a):
        actions = []
        for p in range(self.players):
            action = 0
            if self.use_restricted_actions == retro.Actions.DISCRETE:
                for combo in self.button_combos:
                    current = a % len(combo)
                    a //= len(combo)
                    action |= combo[current]
            elif self.use_restricted_actions == retro.Actions.MULTI_DISCRETE:
                ap = a[self.num_buttons * p:self.num_buttons * (p + 1)]
                for i in range(len(ap)):
                    buttons = self.button_combos[i]
                    action |= buttons[ap[i]]
            else:
                ap = a[self.num_buttons * p:self.num_buttons * (p + 1)]
                for i in range(len(ap)):
                    action |= int(ap[i]) << i
                if self.use_restricted_actions == retro.Actions.FILTERED:
                    action = self.data.filter_action(action)
            ap = np.zeros([self.num_buttons], np.uint8)
            for i in range(self.num_buttons):
                ap[i] = (action >> i) & 1
            actions.append(ap)
        return actions

    def step(self, a):
        if self.img is None:
            raise RuntimeError('Please call env.reset() before env.step()')

        for p, ap in enumerate(self.action_to_array(a)):
            if self.movie:
                for i in range(self.num_buttons):
                    self.movie.set_key(i, ap[i], p)
            self.em.set_button_mask(ap, p)

        if self.movie:
            self.movie.step()
        self.em.step()
        self.img = ob = self.get_screen()
        self.data.update_ram()
        rew, done, info = self.compute_step()
        return ob, rew, bool(done), dict(info)

    def reset(self):
        if self.initial_state:
            self.em.set_state(self.initial_state)
        for p in range(self.players):
            self.em.set_button_mask(np.zeros([self.num_buttons], np.uint8), p)
        self.em.step()
        if self.movie_path is not None:
            rel_statename = os.path.splitext(os.path.basename(self.statename))[0]
            self.record_movie(os.path.join(self.movie_path, '%s-%s-%06d.bk2' % (self.gamename, rel_statename, self.movie_id)))
            self.movie_id += 1
        if self.movie:
            self.movie.step()
        self.img = ob = self.get_screen()
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
            return self.get_screen() if self.img is None else self.img
        elif mode == "human":
            if self.viewer is None:
                from gym.envs.classic_control.rendering import SimpleImageViewer
                self.viewer = SimpleImageViewer()
            self.viewer.imshow(self.img)
            return self.viewer.isopen

    def close(self):
        if hasattr(self, 'em'):
            del self.em

    def get_action_meaning(self, act):
        actions = []
        for p, action in enumerate(self.action_to_array(act)):
            actions.append([self.buttons[i] for i in np.extract(action, np.arange(len(action)))])
        if self.players == 1:
            return actions[0]
        return actions

    def get_screen(self, player=0):
        img = self.em.get_screen()
        x, y, w, h = self.data.crop_info(player)
        if not w or x + w > img.shape[1]:
            w = img.shape[1]
        else:
            w += x
        if not h or y + h > img.shape[0]:
            h = img.shape[0]
        else:
            h += y
        if x == 0 and y == 0 and w == img.shape[1] and h == img.shape[0]:
            return img
        return img[y:h, x:w]

    def load_state(self, statename, inttype=retro.data.Integrations.DEFAULT):
        if not statename.endswith('.state'):
                statename += '.state'

        with gzip.open(retro.data.get_file_path(self.gamename, statename, inttype), 'rb') as fh:
            self.initial_state = fh.read()

        self.statename = statename
