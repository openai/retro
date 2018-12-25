import pytest
import retro.data
import os
import warnings as w
import subprocess

warnings = []
errors = []

games = retro.data.list_games(retro.data.Integrations.ALL)
all_games = []
for g in games:
    overlays = 0
    if os.path.exists(os.path.join(retro.data.path(), *retro.data.Integrations.STABLE.paths, g)):
        all_games.append(g)
        overlays += 1
    if os.path.exists(os.path.join(retro.data.path(), *retro.data.Integrations.EXPERIMENTAL_ONLY.paths, g)):
        all_games.append(g + '-exp')
        overlays += 1
    if os.path.exists(os.path.join(retro.data.path(), *retro.data.Integrations.CONTRIB_ONLY.paths, g)):
        all_games.append(g + '-contrib')
        overlays += 1
    if overlays > 1:
        all_games.append(g + '-all')

inttypes = {
    'exp': retro.data.Integrations.EXPERIMENTAL_ONLY,
    'contrib': retro.data.Integrations.CONTRIB_ONLY,
    'all': retro.data.Integrations.ALL,
}


@pytest.fixture(params=[g.replace('-', '_') for g in all_games])
def game(request):
    game = request.param.split('_')
    return '%s-%s' % (game[0], game[1]), inttypes[game[2]] if len(game) > 2 else retro.data.Integrations.STABLE


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
    import retro.data
    path = os.path.join(os.path.dirname(__file__), '../../tests/roms')

    get_file_path = retro.data.get_file_path
    get_romfile_path = retro.data.get_romfile_path

    retro.data.get_file_path = lambda game, file, *args, **kwargs: os.path.join(path, file)
    retro.data.get_romfile_path = lambda game, *args, **kwargs: [os.path.join(path, g) for g in os.listdir(path) if g.startswith(game)][0]

    env_box = []

    def create(state=retro.State.NONE, *args, **kwargs):
        env = retro.make(request.param, state, *args, **kwargs)
        env_box.append(env)
        return env

    yield create

    if env_box:
        env_box[0].close()
        del env_box[0]

    retro.data.get_file_path = get_file_path
    retro.data.get_romfile_path = get_romfile_path
