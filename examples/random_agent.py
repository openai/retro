#!/usr/bin/env python

import argparse
import retro

parser = argparse.ArgumentParser()
parser.add_argument('game', help='the name or path for the game to run')
parser.add_argument('state', nargs='?', help='the initial state file to load, minus the extension')
parser.add_argument('--scenario', '-s', default='scenario', help='the scenario file to load, minus the extension')
parser.add_argument('--record', '-r', action='store_true', help='record bk2 movies')
parser.add_argument('--verbose', '-v', action='count', default=1, help='increase verbosity (can be specified multiple times)')
parser.add_argument('--quiet', '-q', action='count', default=0, help='decrease verbosity (can be specified multiple times)')
args = parser.parse_args()

env = retro.make(args.game, args.state or retro.STATE_DEFAULT, scenario=args.scenario, record=args.record)
verbosity = args.verbose - args.quiet
try:
    while True:
        ob = env.reset()
        t = 0
        totrew = 0
        while True:
            ac = env.action_space.sample()
            ob, rew, done, info = env.step(ac)
            t += 1
            if t % 10 == 0:
                if verbosity > 1:
                    infostr = ''
                    if info:
                        infostr = ', info: ' + ', '.join(['%s=%i' % (k, v) for k, v in info.items()])
                    print(('t=%i' % t) + infostr)
                env.render()
            totrew += rew
            if verbosity > 0:
                if rew > 0:
                    print('t=%i got reward: %d, current reward: %d' % (t, rew, totrew))
                if rew < 0:
                    print('t=%i got penalty: %d, current reward: %d' % (t, rew, totrew))
            if done:
                env.render()
                try:
                    if verbosity >= 0:
                        print("done! total reward: time=%i, reward=%d" % (t, totrew))
                        input("press enter to continue")
                        print()
                    else:
                        input("")
                except EOFError:
                    exit(0)
                break
except KeyboardInterrupt:
    exit(0)
