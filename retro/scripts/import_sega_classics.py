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
        elif sys.platform.startswith('darwin'):
            r = requests.get('https://steamcdn-a.akamaihd.net/client/installer/steamcmd_osx.tar.gz')
        else:
            raise RuntimeError('Unknown platform %s' % sys.platform)
        tarball = tarfile.open(fileobj=io.BytesIO(r.content))
        tarball.extractall(dir)

        # Steamcmd doesn't like to be used as the target of
        # force_install_dir, and will instead install in the
        # default steam directory
        with tempfile.TemporaryDirectory() as rom_install_dir:
            command = ['%s/steamcmd.sh' % dir,
                       '+login', username,
                       '+force_install_dir', rom_install_dir,
                       '+@sSteamCmdForcePlatformType', 'windows',
                       '+app_update', '34270', 'validate',
                       '+quit']

            print('Installing games...')
            output = subprocess.run(command, input=password.encode('utf-8'), stdout=subprocess.PIPE)
            if output.returncode != 0:
                stdout = output.stdout.decode('utf-8').split('\n')
                print(*stdout[-3:-1], sep='\n')
                sys.exit(1)
            romdir = os.path.join(rom_install_dir, 'uncompressed ROMs')
            roms = [os.path.join(romdir, rom) for rom in os.listdir(romdir)]
            retro.data.merge(*roms, quiet=False)


if __name__ == '__main__':
    main()
