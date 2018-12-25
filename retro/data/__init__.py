from retro._retro import GameDataGlue, RetroEmulator, data_path as _data_path
import glob
import hashlib
import json
import os
import sys

try:
    import enum
    from enum import Flag
except ImportError:
    # Python < 3.6 doesn't support Flag, so we polyfill it ourself
    class Flag(enum.Enum):
        def __and__(self, b):
            value = self.value & b.value
            try:
                return Integrations(value)
            except ValueError:
                return value

        def __or__(self, b):
            value = self.value | b.value
            try:
                return Integrations(value)
            except ValueError:
                return value

__all__ = ['GameData', 'Integrations', 'add_integrations', 'add_custom_integration', 'path', 'get_file_path', 'get_romfile_path', 'list_games', 'list_states', 'merge']

if sys.platform.startswith('linux'):
    EXT = 'so'
elif sys.platform == 'darwin':
    EXT = 'dylib'
elif sys.platform == 'win32':
    EXT = 'dll'
else:
    raise RuntimeError('Unrecognized platform')

DATA_PATH = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))

EMU_CORES = {}
EMU_INFO = {}
EMU_EXTENSIONS = {}


class DefaultIntegrations:
    @classmethod
    def _init(cls):
        if not hasattr(cls, 'DEFAULT'):
            cls.reset()

    def __or__(self, b):
        try:
            self._init()
        except NameError:
            return False
        return DefaultIntegrations.DEFAULT.value | b

    def __and__(self, b):
        try:
            self._init()
        except NameError:
            return False
        return DefaultIntegrations.DEFAULT.value & b

    @classmethod
    def add(cls, extra):
        cls._init()
        cls.DEFAULT |= extra

    @classmethod
    def reset(cls):
        cls.DEFAULT = Integrations.STABLE


class Integrations(Flag):
    STABLE = 1
    EXPERIMENTAL_ONLY = 2
    CONTRIB_ONLY = 4
    CUSTOM_ONLY = 8
    EXPERIMENTAL = EXPERIMENTAL_ONLY | STABLE
    CONTRIB = CONTRIB_ONLY | STABLE
    CUSTOM = CUSTOM_ONLY | STABLE
    ALL = STABLE | EXPERIMENTAL_ONLY | CONTRIB_ONLY | CUSTOM_ONLY
    DEFAULT = DefaultIntegrations()

    @classmethod
    def _init(cls):
        if not hasattr(cls, 'CUSTOM_PATHS'):
            cls.CUSTOM_PATHS = []

    @property
    def paths(self):
        p = []
        if self & self.CONTRIB_ONLY:
            p.append(str(self.CONTRIB_ONLY))
        if self & self.EXPERIMENTAL_ONLY:
            p.append(str(self.EXPERIMENTAL_ONLY))
        if self & self.CUSTOM_ONLY:
            Integrations._init()
            p.extend(self.CUSTOM_PATHS)
        if self & self.STABLE:
            p.append('stable')
        return p

    @classmethod
    def add_custom_path(cls, path):
        cls._init()
        cls.CUSTOM_PATHS.append(path)

    @classmethod
    def clear_custom_paths(cls):
        cls._init()
        del cls.CUSTOM_PATHS[:]

    def __str__(self):
        if self == self.ALL:
            return 'all'
        if self == self.STABLE:
            return ''
        names = []
        if self & self.STABLE:
            names.append('stable')
        if self & self.CONTRIB_ONLY:
            names.append('contrib')
        if self & self.EXPERIMENTAL_ONLY:
            names.append('experimental')
        if self & self.CUSTOM_ONLY:
            names.append('custom')
        return '|'.join(names)


class GameData(GameDataGlue):
    def __init__(self, game=None, data=None, scenario=None, inttype=Integrations.DEFAULT):
        super(GameData, self).__init__()
        if game:
            if not data:
                data = get_file_path(game, 'data.json', inttype)
            if not data.endswith('.json'):
                data += '.json'

            if not os.path.isabs(data):
                data = get_file_path(game, data, inttype)

            if not scenario:
                scenario = get_file_path(game, 'scenario.json', inttype)
            if not scenario.endswith('.json'):
                scenario += '.json'

            if not os.path.isabs(scenario):
                scenario = get_file_path(game, scenario, inttype)
        if data:
            self.load(data, scenario)

    def __getitem__(self, name):
        return self.lookup_value(name)

    def __setitem__(self, name, value):
        return self.set_value(name, value)

    @property
    def searches(self):
        return SearchListHandle(self)

    @property
    def vars(self):
        return Variables(self)


class Variables(object):
    def __init__(self, data):
        super(Variables, self).__init__()
        self.data = data

    def __getitem__(self, name):
        return self.data.get_variable(name)

    def __setitem__(self, name, value):
        return self.data.set_variable(name, value)

    def __delitem__(self, name):
        self.data.remove_variable(name)

    def __iter__(self):
        variables = self.data.list_variables()
        for v in variables.items():
            yield v

    def __contains__(self, name):
        variables = self.data.list_variables()
        return name in variables


class SearchListHandle(object):
    def __init__(self, data):
        self._data = data

    def __getitem__(self, name):
        return SearchHandle(self._data, name)

    def __delitem__(self, name):
        self._data.remove_search(name)

    def __iter__(self):
        searches = self._data.list_searches()
        for search in searches.items():
            yield search

    def __contains__(self, name):
        searches = self._data.list_searches()
        return name in searches

    def load(self, name):
        self._data.load_searches(name)

    def save(self, name):
        self._data.save_searches(name)


class SearchHandle(object):
    def __init__(self, data, name):
        self._data = data
        self._name = name
        self._search = None

    def search(self, value):
        self._data.search(self._name, value)

    def delta(self, op, ref):
        self._data.delta_search(self._name, op, ref)

    def __getattr__(self, attr):
        if not self._search:
            self._search = self._data.get_search(self._name)
        return getattr(self._search, attr)


def add_integrations(integrations):
    DefaultIntegrations.add(integrations)


def add_custom_integration(path):
    DefaultIntegrations.add(Integrations.CUSTOM_ONLY)
    Integrations.add_custom_path(path)


def init_core_info(path):
    for fname in glob.glob(os.path.join(path, '*.json')):
        with open(fname) as f:
            core_info = f.read()
            RetroEmulator.load_core_info(core_info)
            EMU_INFO.update(json.loads(core_info))
            for platform, core in EMU_INFO.items():
                EMU_CORES[platform] = core['lib'] + '_libretro.' + EXT
                for ext in core['ext']:
                    EMU_EXTENSIONS['.' + ext] = platform


def path(hint=DATA_PATH):
    if hint == DATA_PATH and not os.path.exists(os.path.join(DATA_PATH, 'data', 'stable', 'Airstriker-Genesis')):
        # Development installation
        hint = os.path.join(hint, '..')
    return _data_path(hint)


def get_file_path(game, file, inttype=Integrations.DEFAULT):
    """
    Return the path to a given game's directory
    """
    base = path()
    for t in inttype.paths:
        possible_path = os.path.join(base, t, game, file)
        if os.path.exists(possible_path):
            return possible_path

    return None


def get_romfile_path(game, inttype=Integrations.DEFAULT):
    """
    Return the path to a given game's romfile
    """
    for extension in EMU_EXTENSIONS.keys():
        possible_path = get_file_path(game, "rom" + extension, inttype)
        if possible_path:
            return possible_path

    raise FileNotFoundError("No romfiles found for game: %s" % game)


def list_games(inttype=Integrations.DEFAULT):
    files = []
    for curpath in inttype.paths:
        files.extend(os.listdir(os.path.join(path(), curpath)))
    possible_games = []
    for file in files:
        if get_file_path(file, "rom.sha", inttype):
            possible_games.append(file)
    return sorted(set(possible_games))


def list_states(game, inttype=Integrations.DEFAULT):
    paths = []
    for curpath in inttype.paths:
        paths.append(os.path.join(path(), curpath, game))
    states = []
    for curpath in paths:
        local_states = glob.glob(os.path.join(curpath, "*.state"))
        states.extend(os.path.split(local_state)[-1][:-len(".state")]
                      for local_state in local_states
                      if not os.path.split(local_state)[-1].startswith("_"))
    return sorted(set(states))


def list_scenarios(game, inttype=Integrations.DEFAULT):
    paths = []
    for curpath in inttype.paths:
        paths.append(os.path.join(path(), curpath, game))
    scens = []
    for curpath in paths:
        local_json = glob.glob(os.path.join(curpath, "*.json"))
        for j in local_json:
            try:
                with open(j) as f:
                    scen = json.load(f)
            except (json.JSONDecodeError, IOError):
                continue
            if scen.get('reward') is not None or scen.get('rewards') is not None or scen.get('done') is not None:
                scens.append(os.path.split(j)[-1][:-len(".json")])
    return sorted(set(scens))


def parse_smd(header, body):
    import numpy as np
    try:
        if body[0x80] != b'E' or body[0x81] != b'A':
            return header + body
        body2 = b''
        for i in range(len(body) / 0x4000):
            block = body[i * 0x4000:(i + 1) * 0x4000]
            if not block:
                break
            nb = np.fromstring(block, dtype=np.uint8)
            nb = np.flipud(nb.reshape(2, 0x2000))
            nb = nb.flatten(order='F')
            body2 += nb.tostring()
    except IndexError:
        return header + body
    return body2


def groom_rom(rom):
    with open(rom, 'rb') as r:
        if rom.lower().endswith('.smd'):
            # Read Super Magic Drive header
            header = r.read(512)
            body = r.read()
            body = parse_smd(header, body)
        elif rom.lower().endswith('.nes'):
            header = r.read(16)
            body = r.read()
            return header + body, hashlib.sha1(body).hexdigest()
        else:
            # Don't read more than 32 MiB, the largest game supported
            body = r.read(0x2000000)
            if r.read(1):
                raise ValueError('ROM is too big')
    return body, hashlib.sha1(body).hexdigest()


def verify_hash(game, inttype=Integrations.DEFAULT):
    import retro
    errors = []
    rom = get_romfile_path(game, inttype=inttype)
    system = retro.get_romfile_system(rom)
    with open(retro.data.get_file_path(game, 'rom.sha', inttype=inttype | retro.data.Integrations.STABLE)) as f:
        expected_shas = f.read().strip().split('\n')
    with open(rom, 'rb') as f:
        if system == 'Nes':
            # Chop off header for checksum
            f.read(16)
        real_sha = hashlib.sha1(f.read()).hexdigest()
    if real_sha not in expected_shas:
        errors.append((game, 'sha mismatch'))
    return errors


def merge(*args, quiet=True):
    import retro
    known_hashes = {}
    imported_games = 0
    for game in list_games(Integrations.ALL):
        for curpath in Integrations.ALL.paths:
            shafile = os.path.join(path(), curpath, game, 'rom.sha')
            try:
                with open(shafile) as f:
                    shas = f.read().strip().split('\n')
            except (FileNotFoundError, ValueError):
                continue
            for ext, platform in EMU_EXTENSIONS.items():
                if game.endswith('-' + platform):
                    break
            for sha in shas:
                known_hashes[sha] = (game, ext, os.path.join(path(), curpath))
    for rom in args:
        try:
            data, hash = groom_rom(rom)
        except (IOError, ValueError):
            continue
        if hash in known_hashes:
            game, ext, curpath = known_hashes[hash]
            if not quiet:
                print('Importing', game)
            with open(os.path.join(curpath, game, 'rom%s' % ext), 'wb') as f:
                f.write(data)
            imported_games += 1
    if not quiet:
        print('Imported %i games' % imported_games)
