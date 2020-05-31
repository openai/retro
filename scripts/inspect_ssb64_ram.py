"""Notes:

1. for some reason the image observation space doesn't work
- just gives a segfault
- ok let's figure that out?
- what could be wrong?
- nah not yet

"""

import sys
import time

import gym
import matplotlib.pyplot as plt
import numpy as np
import retro

from baselines.common.retro_wrappers import (make_retro, wrap_deepmind_retro, MovieRecord,
                                             RewardScaler)
from baselines.common.atari_wrappers import (WarpFrame, FrameStack, ScaledFloatFrame, MaxAndSkipEnv)


def main1():
    retro.data.add_custom_integration("custom")
    env = retro.RetroEnv(game="SuperSmashBros-N64",
                         inttype=retro.data.Integrations.CUSTOM,
                         obs_type=retro.Observations.IMAGE)
    action = env.action_space.sample()
    state = env.reset()
    plt.imshow(state)
    plt.show()
    # state, reward, terminal, info = env.step(action)

    # player_list_pointer = 0x130D84
    # match_setting_pointer = 0xA50E8

    # print(state[match_setting_pointer:match_setting_pointer + 64])

    # import ipdb
    # ipdb.set_trace()


def load_starting_ram_for_state(state):
    env = retro.RetroEnv(game="SuperSmashBros-N64",
                         state=state,
                         inttype=retro.data.Integrations.CUSTOM,
                         obs_type=retro.Observations.RAM)
    action = env.action_space.sample()
    state = env.reset()
    state, reward, terminal, info = env.step(action)
    return state


def main2():
    retro.data.add_custom_integration("custom")
    player_list_pointer = 0x130D84
    match_setting_pointer = 0xA50E8
    player_size = 0xB50

    dreamland = load_starting_ram_for_state("ssb64.pikachu.level9dk.dreamland")
    np.save("/home/wulfebw/programming/retro/save_states/dreamland.npy", dreamland)

    peaches = load_starting_ram_for_state("ssb64.pikachu.level9dk.peaches_castle")
    np.save("/home/wulfebw/programming/retro/save_states/peaches.npy", peaches)

    peaches = load_starting_ram_for_state("kongo")
    np.save("/home/wulfebw/programming/retro/save_states/kongo.npy", peaches)

    peaches = load_starting_ram_for_state("ssb64.pikachu.level9dk.sector_z")
    np.save("/home/wulfebw/programming/retro/save_states/sector_z.npy", peaches)

    peaches = load_starting_ram_for_state("yoshi.level3dk.level3pikachu.level3fox.dreamland")
    np.save("/home/wulfebw/programming/retro/save_states/4_player.npy", peaches)

    peaches = load_starting_ram_for_state(
        "ssb64.pikachu_6_dmg_16_lives.dk_45_dmg_17_lives.dreamland")
    np.save("/home/wulfebw/programming/retro/save_states/dmg_lives.npy", peaches)

    peaches = load_starting_ram_for_state("ssb64.mario.level3samus.hyrule")
    np.save("/home/wulfebw/programming/retro/save_states/dmg_lives_2.npy", peaches)

    ptr = player_list_pointer
    player_1_data = [hex(b) for b in dreamland[ptr:ptr + player_size]]
    player_2_data = [hex(b) for b in dreamland[ptr + player_size:ptr + 2 * player_size]]


def main3():
    retro.data.add_custom_integration("custom")
    env = retro.RetroEnv(game="SuperSmashBros-N64",
                         inttype=retro.data.Integrations.CUSTOM,
                         obs_type=retro.Observations.IMAGE)
    start = time.time()
    env.reset()
    num_steps = 200
    for i in range(num_steps):
        sys.stdout.write(f"\r{i+1} / {num_steps}")
        action = env.action_space.sample()
        env.step(action)
        env.render()
    end = time.time()
    print(end - start)


def main4():
    retro.data.add_custom_integration("custom")
    env = retro.n64_env.N64Env(game="SuperSmashBros-N64",
                               use_restricted_actions=retro.Actions.MULTI_DISCRETE,
                               inttype=retro.data.Integrations.CUSTOM,
                               obs_type=retro.Observations.IMAGE)
    start = time.time()
    env.reset()
    num_steps = 20000

    action = np.array([0, 0, 0])

    for i in range(num_steps):
        sys.stdout.write(f"\r{i+1} / {num_steps}")
        obs, reward, done, info = env.step(action)

        print(reward)
        print(done)

        env.render()
    end = time.time()
    print(end - start)


class N64Interactive(retro.Interactive):
    """
    Interactive setup for retro games
    """
    def __init__(self, env):
        self._buttons = env.buttons
        super().__init__(env=env, sync=False, tps=60, aspect_ratio=4 / 3)

    def get_image(self, _obs, env):
        return env.render(mode='rgb_array')

    def keys_to_act(self, keys):
        inputs = {
            None: False,
            'BUTTON': 'Z' in keys,
            'A': 'Z' in keys,
            'B': 'X' in keys,
            'C': 'C' in keys,
            'X': 'A' in keys,
            'Y': 'S' in keys,
            'Z': 'D' in keys,
            'L': 'Q' in keys,
            'R': 'W' in keys,
            'UP': 'UP' in keys,
            'DOWN': 'DOWN' in keys,
            'LEFT': 'LEFT' in keys,
            'RIGHT': 'RIGHT' in keys,
            'MODE': 'TAB' in keys,
            'SELECT': 'TAB' in keys,
            'RESET': 'ENTER' in keys,
            'START': 'ENTER' in keys,
        }
        return [inputs[b] for b in self._buttons]


def main5():
    retro.data.add_custom_integration("custom")
    env = retro.n64_env.N64Env(game="SuperSmashBros-N64",
                               use_restricted_actions=retro.Actions.ALL,
                               inttype=retro.data.Integrations.CUSTOM,
                               obs_type=retro.Observations.IMAGE)
    ie = N64Interactive(env)
    ie.run()


def main6():
    def wrap_deepmind_n64(env, reward_scale=1 / 100.0, frame_stack=4):
        env = MaxAndSkipEnv(env, skip=4)
        env = WarpFrame(env, width=150, height=100)
        env = FrameStack(env, frame_stack)
        env = ScaledFloatFrame(env)
        env = RewardScaler(env, scale=1 / 100.0)
        return env

    retro.data.add_custom_integration("custom")
    env = retro.n64_env.N64Env(game="SuperSmashBros-N64",
                               use_restricted_actions=retro.Actions.MULTI_DISCRETE,
                               inttype=retro.data.Integrations.CUSTOM,
                               obs_type=retro.Observations.IMAGE)
    env = wrap_deepmind_n64(env)
    env.reset()
    num_steps = 20000
    action = np.array([0, 0, 0])
    for i in range(num_steps):
        sys.stdout.write(f"\r{i+1} / {num_steps}")
        action = env.action_space.sample()
        obs, reward, done, info = env.step(action)
        if done:
            env.reset()
        # env.render()

        if i % 50 == 0:
            fig, axs = plt.subplots(nrows=2, ncols=2, figsize=(12, 12))
            for j in range(4):
                row = j // 2
                col = j % 2
                print(row)
                print(col)
                axs[row, col].imshow(obs[:, :, j])
            plt.show()
            plt.close()
    end = time.time()
    print(end - start)

    return env


if __name__ == "__main__":
    main6()
