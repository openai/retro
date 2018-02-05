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
    def run(fn, arg):
        global pool
        try:
            future = pool.submit(fn, arg)
            return future.result(2)
        except BrokenProcessPool:
            pool = ProcessPoolExecutor(1)
            return [], [(arg, 'subprocess crashed')]
        except TimeoutError:
            return [], [(arg, 'task timed out')]
    yield run
    pool.shutdown()


def load(game):
    errors = []
    rom = retro.get_romfile_path(game)
    emu = retro.RetroEmulator(rom)

    emu.step()

    del emu
    gc.collect()

    return [], errors


def state(game):
    errors = []
    states = retro.list_states(game)
    if not states:
        return [], []

    rom = retro.get_romfile_path(game)
    path = retro.get_game_path(game)
    emu = retro.RetroEmulator(rom)
    for statefile in states:
        try:
            with gzip.open(os.path.join(path, statefile + '.state'), 'rb') as fh:
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
    warnings, errors = processpool(load, game)
    handle(warnings, errors)


def test_state(game, processpool):
    warnings, errors = processpool(state, game)
    handle(warnings, errors)
