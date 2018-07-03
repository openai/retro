import pytest
import retro.data

inttypes = {
    'exp': retro.data.Integrations.EXPERIMENTAL_ONLY,
    'contrib': retro.data.Integrations.CONTRIB_ONLY,
}


def pytest_collection_modifyitems(items):
    def test(*args, **kwargs):
        print(kwargs)
        return False

    for item in items:
        if item.originalname in ('test_load', 'test_rom', 'test_state', 'test_hash'):
            for key in item.keywords.keys():
                if '[' + key + ']' not in item.nodeid:
                    continue

                game = key.split('_')
                gamename = '%s-%s' % (game[0], game[1])
                try:
                    retro.data.get_romfile_path(gamename, inttypes[game[2]] if len(game) > 2 else retro.data.Integrations.STABLE)
                except (FileNotFoundError, KeyError):
                    item.add_marker(pytest.mark.skip)
