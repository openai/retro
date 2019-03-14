import retro

def main():
    env = retro.make(game='Pong-Atari2600', players=2)
    obs = env.reset()
    while True:
        # action_space will by MultiBinary(16) now instead of MultiBinary(8)
        # the bottom half of the actions will be for player 1 and the top half for player 2
        obs, rew, done, info = env.step(env.action_space.sample())
        # rew will be a list of [player_1_rew, player_2_rew]
        # done and info will remain the same
        env.render()
        if done:
            obs = env.reset()
    env.close()


if __name__ == "__main__":
    main()
