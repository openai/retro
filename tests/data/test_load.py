import retro
import pytest
import gc
import gzip
import os
import zlib
from retro.testing import game, handle
from concurrent.futures import ProcessPoolExecutor, TimeoutError
from concurrent.futures.process import BrokenProcessPool

pool = ProcessPoolExecutor(1)


@pytest.fixture(scope="module")
def processpool():
    def run(fn, *args):
        global pool
        try:
            future = pool.submit(fn, *args)
            return future.result(2)
        except BrokenProcessPool:
            pool = ProcessPoolExecutor(1)
            return [], [(args[0], 'subprocess crashed')]
        except TimeoutError:
            return [], [(args[0], 'task timed out')]
    yield run
    pool.shutdown()


def load(game, inttype):
    errors = []
    rom = retro.data.get_romfile_path(game, inttype)
    emu = retro.RetroEmulator(rom)

    emu.step()

    del emu
    gc.collect()

    return [], errors


def state(game, inttype):
    errors = []
    states = retro.data.list_states(game, inttype)
    if not states:
        return [], []

    rom = retro.data.get_romfile_path(game, inttype | retro.data.Integrations.STABLE)
    emu = retro.RetroEmulator(rom)
    for statefile in states:
        try:
            with gzip.open(retro.data.get_file_path(game, statefile + '.state', inttype), 'rb') as fh:
                state = fh.read()
        except (IOError, zlib.error):
            errors.append((game, 'state failed to decode: %s' % statefile))
            continue

        emu.set_state(state)
        emu.step()

    del emu
    gc.collect()

    return [], errors


def test_load(game, processpool):
    warnings, errors = processpool(load, *game)
    handle(warnings, errors)


def test_state(game, processpool):
    warnings, errors = processpool(state, *game)
    handle(warnings, errors)
