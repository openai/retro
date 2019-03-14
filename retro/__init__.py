import os
import retro.data

from retro._retro import Movie, RetroEmulator, core_path
from retro.enums import Actions, State, Observations
from retro.retro_env import RetroEnv


ROOT_DIR = os.path.abspath(os.path.dirname(__file__))
core_path(os.path.join(os.path.dirname(__file__), 'cores'))

for path in ('VERSION.txt', '../VERSION'):
    try:
        with open(os.path.join(os.path.dirname(__file__), path)) as f:
            __version__ = f.read()
            break
    except IOError:
        pass

__all__ = ['Movie', 'RetroEmulator', 'Actions', 'State', 'Observations', 'get_core_path', 'get_romfile_system', 'get_system_info', 'make', 'RetroEnv']

retro.data.init_core_info(core_path())


def get_core_path(corename):
    return os.path.join(core_path(), retro.data.EMU_CORES[corename])


def get_romfile_system(rom_path):
    extension = os.path.splitext(rom_path)[1]
    if extension in retro.data.EMU_EXTENSIONS:
        return retro.data.EMU_EXTENSIONS[extension]
    else:
        raise Exception("Unsupported rom type at path: {}".format(rom_path))


def get_system_info(system):
    if system in retro.data.EMU_INFO:
        return retro.data.EMU_INFO[system]
    else:
        raise KeyError("Unsupported system type: {}".format(system))


def make(game, state=State.DEFAULT, inttype=retro.data.Integrations.DEFAULT, **kwargs):
    """
    Create a Gym environment for the specified game
    """
    try:
        retro.data.get_romfile_path(game, inttype)
    except FileNotFoundError:
        if not retro.data.get_file_path(game, "rom.sha", inttype):
            raise
        else:
            raise FileNotFoundError('Game not found: %s. Did you make sure to import the ROM?' % game)
    return RetroEnv(game, state, inttype=inttype, **kwargs)
