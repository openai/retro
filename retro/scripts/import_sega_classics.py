#!/usr/bin/env python
import getpass
import io
import os
import requests
import retro.data
import subprocess
import sys
import tarfile
import tempfile
import zipfile


def main():
    username = input('Steam Username: ')
    password = getpass.getpass('Steam Password (leave blank if cached): ')

    if password:
        password = password + '\n'

        authcode = input('Steam Guard code: ')
        if authcode:
            password = password + authcode + '\n'
        else:
            password = password + '\r\n'
    else:
        password = '\r\n'

    with tempfile.TemporaryDirectory() as dir:
        if sys.platform.startswith('linux'):
            r = requests.get('https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz')
            steamcmd = 'steamcmd.sh'
        elif sys.platform.startswith('darwin'):
            r = requests.get('https://steamcdn-a.akamaihd.net/client/installer/steamcmd_osx.tar.gz')
            steamcmd = 'steamcmd.sh'
        elif sys.platform.startswith('win'):
            r = requests.get('https://steamcdn-a.akamaihd.net/client/installer/steamcmd.zip')
            steamcmd = 'steamcmd.exe'
        else:
            raise RuntimeError('Unknown platform %s' % sys.platform)
        if sys.platform.startswith('win'):
            zipf = zipfile.ZipFile(io.BytesIO(r.content))
            zipf.extractall(dir)
        else:
            tarball = tarfile.open(fileobj=io.BytesIO(r.content))
            tarball.extractall(dir)

        # Steamcmd doesn't like to be used as the target of
        # force_install_dir, and will instead install in the
        # default steam directory
        with tempfile.TemporaryDirectory() as rom_install_dir:
            command = [os.path.join(dir, steamcmd),
                       '+login', username,
                       '+force_install_dir', rom_install_dir,
                       '+@sSteamCmdForcePlatformType', 'windows',
                       '+app_update', '34270', 'validate',
                       '+quit']

            print('Downloading games...')
            output = subprocess.run(command, input=password.encode('utf-8'), stdout=subprocess.PIPE)
            if output.returncode not in (0, 7):
                stdout = output.stdout.decode('utf-8').split('\n')
                print(*stdout[-3:-1], sep='\n')
                sys.exit(1)
            roms = []
            print('Installing games...')
            for base, _, files in os.walk(rom_install_dir):
                if not base.endswith('uncompressed ROMs'):
                    continue
                roms.extend([os.path.join(base, file) for file in files])
            retro.data.merge(*roms, quiet=False)


if __name__ == '__main__':
    main()
