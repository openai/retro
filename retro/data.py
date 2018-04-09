from retro._retro import GameDataGlue
import os


class GameData(GameDataGlue):
    def __getitem__(self, name):
        return self.get_variable(name)

    def __setitem__(self, name, value):
        return self.set_variable(name, value)

    def __delitem__(self, name):
        self.remove_variable(name)

    def __iter__(self):
        variables = self.list_variables()
        for v in variables.items():
            yield v

    def __contains__(self, name):
        variables = self.list_variables()
        return name in variables

    @property
    def memory(self):
        return super(GameData, self).memory()


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
    import hashlib
    with open(rom, 'rb') as r:
        # Read Super Magic Drive header
        header = r.read(512)
        body = r.read()
    body = parse_smd(header, body)
    header = b''
    return body, hashlib.sha1(body).hexdigest()


def merge(*args, quiet=True):
    import retro
    known_hashes = {}
    imported_games = 0
    for game in retro.list_games():
        shafile = os.path.join(retro.get_game_path(game), 'rom.sha')
        with open(shafile) as f:
            shas = f.read().strip().split('\n')
        for ext, platform in retro.EMU_EXTENSIONS.items():
            if game.endswith('-' + platform):
                break
        for sha in shas:
            known_hashes[sha] = (game, ext)
    for rom in args:
        try:
            data, hash = groom_rom(rom)
        except IOError:
            continue
        if hash in known_hashes:
            game, ext = known_hashes[hash]
            if not quiet:
                print('Importing', game)
            with open(os.path.join(retro.get_game_path(game), 'rom%s' % ext), 'wb') as f:
                f.write(data)
            imported_games += 1
    if not quiet:
        print('Imported %i games' % imported_games)
