#!/usr/bin/env python
import retro.data
import os
import sys
import zipfile


def _check_zipfile(f, process_f):
    with zipfile.ZipFile(f) as zf:
        for entry in zf.infolist():
            _root, ext = os.path.splitext(entry.filename)
            with zf.open(entry) as innerf:
                if ext == ".zip":
                    _check_zipfile(innerf, process_f)
                else:
                    process_f(entry.filename, innerf)


def main():
    paths = sys.argv[1:] or ['.']
    known_hashes = retro.data.get_known_hashes()

    imported_games = 0

    def save_if_matches(filename, f):
        nonlocal imported_games
        try:
            data, hash = retro.data.groom_rom(filename, f)
        except (IOError, ValueError):
            return
        if hash in known_hashes:
            game, ext, curpath = known_hashes[hash]
            print('Importing', game)
            with open(os.path.join(curpath, game, 'rom%s' % ext), 'wb') as f:
                f.write(data)
            imported_games += 1

    for path in paths:
        for root, dirs, files in os.walk(path):
            for filename in files:
                filepath = os.path.join(root, filename)
                with open(filepath, "rb") as f:
                    _root, ext = os.path.splitext(filename)
                    if ext == ".zip":
                        try:
                            _check_zipfile(f, save_if_matches)
                        except zipfile.BadZipFile:
                            pass
                    else:
                        save_if_matches(filename, f)

    print('Imported %i games' % imported_games)




if __name__ == '__main__':
    main()
