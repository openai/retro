import glob
import gym
import gzip
import os
import json
import subprocess
import sys
from retro._retro import Movie, RetroEmulator, core_path, data_path as _data_path

ROOT_DIR = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
RETRO_DATA_PATH = os.path.dirname(__file__)
core_path(os.path.join(os.path.dirname(__file__), 'cores'))

for path in ('VERSION.txt', '../VERSION'):
    try:
        with open(os.path.join(os.path.dirname(__file__), path)) as f:
            __version__ = f.read()
            break
    except IOError:
        pass

if sys.platform.startswith('linux'):
    EXT = 'so'
elif sys.platform == 'darwin':
    EXT = 'dylib'
elif sys.platform == 'win32':
    EXT = 'dll'
else:
    raise RuntimeError('Unrecognized platform')

ACTIONS_ALL = 0
ACTIONS_FILTERED = 1
ACTIONS_DISCRETE = 2
ACTIONS_MULTI_DISCRETE = 3

STATE_DEFAULT = -1
STATE_NONE = 0

EMU_CORES = {}
EMU_INFO = {}
EMU_EXTENSIONS = {}


def data_path(hint=RETRO_DATA_PATH):
    return _data_path(hint)


with open(os.path.join(os.path.dirname(__file__), 'cores.json')) as f:
    _coreInfo = f.read()
    RetroEmulator.load_core_info(_coreInfo)
    EMU_INFO = json.loads(_coreInfo)
    for platform, core in EMU_INFO.items():
        EMU_CORES[platform] = core['lib'] + '_libretro.' + EXT
        for ext in core['ext']:
            EMU_EXTENSIONS['.' + ext] = platform


def get_core_path(corename):
    return os.path.join(core_path(), EMU_CORES[corename])


def get_game_path(game=""):
    """
    Return the path to a given game's directory
    """
    return os.path.join(data_path(), game)


def get_romfile_path(game):
    """
    Return the path to a given game's romfile
    """
    for extension in EMU_EXTENSIONS.keys():
        possible_path = os.path.join(get_game_path(game), "rom" + extension)
        if os.path.exists(possible_path):
            return possible_path

    raise FileNotFoundError("No romfiles found for game: %s" % game)


def get_romfile_system(rom_path):
    extension = os.path.splitext(rom_path)[1]
    if extension in EMU_EXTENSIONS:
        return EMU_EXTENSIONS[extension]
    else:
        raise Exception("Unsupported rom type at path: {}".format(rom_path))


def get_system_info(system):
    if system in EMU_INFO:
        return EMU_INFO[system]
    else:
        raise KeyError("Unsupported system type: {}".format(system))


def list_games():
    files = os.listdir(get_game_path())
    possible_games = []
    for file in files:
        if os.path.exists(os.path.join(get_game_path(file), "rom.sha")):
            possible_games.append(file)
    return possible_games


def list_states(game):
    path = get_game_path(game)
    states = glob.glob(os.path.join(path, "*.state"))
    return [state.split(os.sep)[-1][:-len(".state")] for state in states if not state.startswith("_")]


def make(game, state=STATE_DEFAULT, **kwargs):
    from retro.retro_env import RetroEnv
    try:
        get_romfile_path(game)
    except FileNotFoundError:
        if not os.path.exists(os.path.join(get_game_path(game), "rom.sha")):
            raise FileNotFoundError('Game not found: %s' % game)
        else:
            raise FileNotFoundError('Game not found: %s. Did you make sure to import the ROM?' % game)
    return RetroEnv(game, state, **kwargs)
