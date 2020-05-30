"""Notes:

1. for some reason the image observation space doesn't work
- just gives a segfault
- ok let's figure that out?
- what could be wrong?
- nah not yet

"""

import gym
import matplotlib.pyplot as plt
import numpy as np
import retro
import sys
import time


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

    peaches = load_starting_ram_for_state(
        "ssb64.mario.level3samus.hyrule")
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


if __name__ == "__main__":
    main2()
