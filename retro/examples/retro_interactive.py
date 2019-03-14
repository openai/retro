import argparse

import retro
from baselines.common.vec_env.subproc_vec_env import SubprocVecEnv
from .interactive import Interactive


class RetroInteractive(Interactive):
    """
    Interactive setup for retro games
    """
    def __init__(self, game, state, scenario):
        def make_env():
            return retro.make(game=game, state=state, scenario=scenario)

        env = make_env()
        self._buttons = env.buttons
        env.close()
        venv = SubprocVecEnv([make_env])
        super().__init__(venv=venv, sync=False, tps=60, aspect_ratio=4/3)

    def get_screen(self, _obs, venv):
        return venv.render(mode='rgb_array')

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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--game', default='SonicTheHedgehog-Genesis')
    parser.add_argument('--state', default=retro.State.DEFAULT)
    parser.add_argument('--scenario', default='scenario')
    args = parser.parse_args()

    ia = RetroInteractive(game=args.game, state=args.state, scenario=args.scenario)
    ia.run()


if __name__ == '__main__':
    main()
