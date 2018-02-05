#!/usr/bin/env python
import pytest
import retro.testing as testdata
import subprocess
import sys

if len(sys.argv) == 2:
    branches = [sys.argv[1]]
elif len(sys.argv) == 3:
    branches = [sys.argv[1], sys.argv[2]]
else:
    branches = ['master']

check = testdata.branch_new(*branches)
if check:
    args = ['-q', '--tb=no', '--disable-warnings', '-k', ' or '.join(check)]
    pytest.main(args)
    for context, error in testdata.errors:
        print('\33[31mE: %s: %s\33[0m' % (context, error))
    for context, warning in testdata.warnings:
        print('\33[33mW: %s: %s\33[0m' % (context, warning))
    if testdata.errors:
        sys.exit(1)
