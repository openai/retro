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


def main():
    username = input('Steam Username: ')
    password = getpass.getpass('Steam Password (leave blank if cached): ')

    if password:
        password = password + '\n'

        authcode = input('Steam Guard code (leave blank if unknown): ')
        if authcode:
            password = password + authcode + '\n'

    with tempfile.TemporaryDirectory() as dir:
        if sys.platform.startswith('linux'):
            r = requests.get('https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz')
        elif sys.platform.startswith('darwin'):
            r = requests.get('https://steamcdn-a.akamaihd.net/client/installer/steamcmd_osx.tar.gz')
        else:
            raise RuntimeError('Unknown platform %s' % sys.platform)
        tarball = tarfile.open(fileobj=io.BytesIO(r.content))
        tarball.extractall(dir)

        command = ['%s/steamcmd.sh' % dir,
                   '+login', username,
                   '+force_install_dir', dir,
                   '+@sSteamCmdForcePlatformType', 'windows',
                   '+app_update', '34270', 'validate',
                   '+quit']

        print('Installing games...')
        subprocess.run(command, check=True, input=password.encode('utf-8'), stdout=subprocess.DEVNULL)
        romdir = os.path.join(dir, 'uncompressed ROMs')
        roms = [os.path.join(romdir, rom) for rom in os.listdir(romdir)]
        retro.data.merge(*roms, quiet=False)


if __name__ == '__main__':
    main()
