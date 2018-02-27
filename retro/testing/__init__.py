import pytest
import retro
import os
import warnings as w
import subprocess

warnings = []
errors = []


@pytest.fixture(params=[g.replace('-', '_') for g in retro.list_games()])
def game(request):
    return request.param.replace('_', '-')


def error(test, info):
    global errors
    errors.append((test, info))


def warn(test, info):
    global warnings
    w.warn('%s: %s' % (test, info))
    warnings.append((test, info))


def handle(warnings, errors):
    for warning in warnings:
        warn(*warning)
    for err in errors:
        error(*err)
    assert not errors


def branch_new(upstream='master', downstream=None):
    branches = [upstream]
    if downstream:
        branches.append(downstream)
    try:
        diffs = subprocess.check_output(['git', 'diff', '--name-only'] + branches).decode('utf-8')
    except subprocess.CalledProcessError:
        return []
    check = {name.split('/')[0].replace('-', '_') for name in diffs.splitlines() if '-' in name}
    return list(check)


@pytest.yield_fixture(params=[os.path.splitext(g)[0] for g in os.listdir(os.path.join(os.path.dirname(__file__), '../../tests/roms'))])
def testenv(request):
    import retro
    path = os.path.join(os.path.dirname(__file__), '../../tests/roms')

    get_game_path = retro.get_game_path
    get_romfile_path = retro.get_romfile_path

    retro.get_game_path = lambda game: path
    retro.get_romfile_path = lambda game: [os.path.join(path, g) for g in os.listdir(path) if g.startswith(game)][0]

    env_box = []

    def create(state=retro.STATE_NONE, *args, **kwargs):
        env = retro.make(request.param, state, *args, **kwargs)
        env_box.append(env)
        return env

    yield create

    if env_box:
        env_box[0].close()
        del env_box[0]

    retro.get_game_path = get_game_path
    retro.get_romfile_path = get_romfile_path
