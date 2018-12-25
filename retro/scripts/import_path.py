#!/usr/bin/env python
import retro.data
import os
import sys


def main():
    potential_roms = []
    paths = sys.argv[1:] or ['.']
    for path in paths:
        if os.path.isdir(path):
            for base, _, files in os.walk(path):
                potential_roms.extend(os.path.join(base, file) for file in files)
        else:
            potential_roms.append(path)

    print('Importing %i potential games...' % len(potential_roms))
    retro.data.merge(*potential_roms, quiet=False)


if __name__ == '__main__':
    main()
