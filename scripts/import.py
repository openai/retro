#!/usr/bin/env python
import retro.data
import os
import sys

potential_roms = []
for path in sys.argv[1:]:
    for base, _, files in os.walk(sys.argv[1]):
        potential_roms.extend([os.path.join(base, file) for file in files])

print('Importing %i potential games...' % len(potential_roms))
retro.data.merge(*potential_roms, quiet=False)
