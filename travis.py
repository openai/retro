import subprocess
import os
import shlex
import json
import glob


class Fold:
    def __init__(self, name, description=None):
        self.fold_name = name
        self.description = description or name

    def __enter__(self):
        print('travis_fold:start:%s\033[33;1m%s\033[0m' % (self.fold_name, self.description), flush=True)

    def __exit__(self, exc_type, exc_value, traceback):
        print('\ntravis_fold:end:%s\r' % self.fold_name, end='', flush=True)


def call(cmd):
    print(' '.join(shlex.quote(p) for p in cmd), flush=True)
    subprocess.check_call(cmd)


def get_bucket():
    from google.oauth2 import service_account
    from google.cloud import storage

    credentials_info = json.loads(os.environ['GOOGLE_APPLICATION_CREDENTIALS_DATA'])
    credentials = service_account.Credentials.from_service_account_info(credentials_info)
    client = storage.Client(credentials=credentials, project=credentials_info['project_id'])
    return client.get_bucket('gym-retro')


def upload_to_gcs(patterns, dest):
    bucket = get_bucket()
    for pattern in patterns:
        print('uploading %s to %s' % (pattern, dest), flush=True)
        for filepath in glob.glob(pattern):
            blob_name = '%s/%s' % (dest, os.path.basename(filepath))
            blob = bucket.blob(blob_name)
            blob.upload_from_filename(filename=filepath)
            blob.make_public()
            print('uploaded ', blob.public_url.replace('%2F', '/'))  # public_url escapes slashes but that breaks pip install


def test():
    import pytest
    import retro.testing as testdata
    args = []
    if os.environ['TRAVIS_BRANCH'] != 'master' or os.environ['TRAVIS_PULL_REQUEST'] != 'false':
        check = testdata.branch_new('origin/master')
        if check:
            args.extend(['-k', ' or '.join(check)])
    pytest.main(args)
    return not testdata.errors


def plot_history():
    import scripts.plot_history as plot
    plot.main()


def main():
    os_name = os.environ['TRAVIS_OS_NAME']
    with Fold('script.deps', 'Installing dependencies'):
        if os_name == 'osx':
            # update brew first, which should install the correct version of ruby
            # and avoid the error "Homebrew must be run under Ruby 2.3"
            call(['brew', 'update'])
            call(['brew', 'install', 'libzip', 'lua@5.1', 'ccache'])
        elif os_name == 'linux':
            cmake_options = []
            os.environ['PATH'] = '/usr/lib/ccache:' + os.environ['PATH']

    with Fold('script.build', 'Building'):
        call(['cmake', '.', '-DBUILD_TESTS=ON'])
        call(['python', 'setup.py', '-q', 'build_ext', '-i', '-j3'])
        call(['pip', 'install', '-e', '.'])
        call(['make', '-j3'])

    if os.environ['TRAVIS_BRANCH'] == 'master' or os.environ['TRAVIS_TAG']:
        with Fold('script.package', 'Packaging binaries'):
            call(['python', 'setup.py', '-q', 'bdist_wheel'])
            upload_to_gcs(['dist/*.whl'], 'builds')

    with Fold('script.test', 'Running tests'):
        call(['ctest', '--verbose'])

        if os_name == 'linux':
            passed = test()
            assert passed


if __name__ == '__main__':
    main()
