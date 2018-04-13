from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import sys
import os
import shutil

VERSION_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'VERSION')

if not os.path.exists(os.path.join(os.path.dirname(__file__), '.git')):
    use_scm_version = False
    shutil.copy('VERSION', 'retro/VERSION.txt')
else:
    def version_scheme(version):
        with open(VERSION_PATH) as v:
            version_file = v.read()
        if version.distance:
            version_file += '.dev%d' % version.distance
        return version_file

    def local_scheme(version):
        v = ''
        if version.distance:
            v = '+' + version.node
        return v
    use_scm_version = {'write_to': 'retro/VERSION.txt',
                       'version_scheme': version_scheme,
                       'local_scheme': local_scheme}


class CMakeBuild(build_ext):
    def run(self):
        suffix = super(CMakeBuild, self).get_ext_filename('')
        pyext_suffix = '-DPYEXT_SUFFIX:STRING=%s' % suffix
        pylib_dir = ''
        if not self.inplace:
            pylib_dir = '-DPYLIB_DIRECTORY:PATH=%s' % self.build_lib
        python_executable = '-DPYTHON_EXECUTABLE:STRING=%s' % sys.executable
        subprocess.check_call(['cmake', '.', '-G', 'Unix Makefiles', pyext_suffix, pylib_dir, python_executable])
        if self.parallel:
            jobs = ['-j%d' % self.parallel]
        else:
            import multiprocessing
            jobs = ['-j%d' % multiprocessing.cpu_count()]
        subprocess.check_call(['make'] + jobs + ['retro'])


setup(
    name='gym-retro',
    author='OpenAI',
    author_email='vickipfau@openai.com',
    url='https://github.com/openai/retro',
    version=open(VERSION_PATH, 'r').read(),
    license='MIT',
    install_requires=['gym'],
    ext_modules=[Extension('retro._retro', ['CMakeLists.txt', 'src/*.cpp'])],
    cmdclass={'build_ext': CMakeBuild},
    packages=['retro', 'retro.data', 'retro.scripts', 'retro.import'],
    package_data={
        'retro': ['cores.json', 'cores/*_libretro*', 'VERSION.txt', 'README.md', 'LICENSES.md'],
        'retro.data': ['*-%s/*' % plat for plat in ['Genesis', 'Atari2600']],
    },
    package_dir={
        'retro.data': 'data'
    },
    setup_requires=['setuptools_scm'],
    use_scm_version=use_scm_version
)
