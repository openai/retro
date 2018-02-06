import pytest
import retro


def pytest_collection_modifyitems(items):
    def test(*args, **kwargs):
        print(kwargs)
        return False

    for item in items:
        if item.originalname in ('test_load', 'test_rom', 'test_state', 'test_hash'):
            for key in item.keywords.keys():
                if '[' + key + ']' not in item.nodeid:
                    continue
                try:
                    retro.get_romfile_path(key.replace('_', '-'))
                except FileNotFoundError:
                    item.add_marker(pytest.mark.skip)
