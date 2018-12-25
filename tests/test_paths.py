import retro
import os
import pytest


@pytest.yield_fixture
def custom_cleanup():
	retro.data.Integrations.clear_custom_paths()
	assert not retro.data.Integrations.CUSTOM_ONLY.paths

	yield

	retro.data.Integrations.clear_custom_paths()
	assert not retro.data.Integrations.CUSTOM_ONLY.paths


def test_basic_paths():
	assert retro.data.Integrations.STABLE.paths == ['stable']
	assert retro.data.Integrations.CONTRIB_ONLY.paths == ['contrib']
	assert retro.data.Integrations.EXPERIMENTAL_ONLY.paths == ['experimental']
	assert not retro.data.Integrations.CUSTOM_ONLY.paths

	assert retro.data.Integrations.CONTRIB.paths == ['contrib', 'stable']
	assert retro.data.Integrations.EXPERIMENTAL.paths == ['experimental', 'stable']
	assert retro.data.Integrations.CUSTOM.paths == ['stable']

	assert retro.data.Integrations.ALL.paths == ['contrib', 'experimental', 'stable']


def test_custom_path(custom_cleanup):
	assert not retro.data.Integrations.CUSTOM_ONLY.paths
	assert retro.data.Integrations.CUSTOM.paths == ['stable']

	retro.data.Integrations.add_custom_path('a')
	assert retro.data.Integrations.CUSTOM_ONLY.paths == ['a']
	assert retro.data.Integrations.CUSTOM.paths == ['a', 'stable']

	retro.data.Integrations.add_custom_path('b')
	assert retro.data.Integrations.CUSTOM_ONLY.paths == ['a', 'b']
	assert retro.data.Integrations.CUSTOM.paths == ['a', 'b', 'stable']


def test_custom_path_default(custom_cleanup):
	assert not retro.data.Integrations.CUSTOM_ONLY.paths
	assert retro.data.Integrations.CUSTOM.paths == ['stable']
	assert retro.data.Integrations.DEFAULT.paths == ['stable']

	retro.data.add_custom_integration('a')
	assert retro.data.Integrations.CUSTOM_ONLY.paths == ['a']
	assert retro.data.Integrations.CUSTOM.paths == ['a', 'stable']
	assert retro.data.Integrations.DEFAULT.paths == ['a', 'stable']

	retro.data.DefaultIntegrations.reset()
	assert retro.data.Integrations.CUSTOM_ONLY.paths == ['a']
	assert retro.data.Integrations.CUSTOM.paths == ['a', 'stable']
	assert retro.data.Integrations.DEFAULT.paths == ['stable']


def test_custom_path_absolute(custom_cleanup):
	assert not retro.data.get_file_path('', 'Dekadence-Dekadrive.md', inttype=retro.data.Integrations.CUSTOM_ONLY)

	test_rom_dir = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'roms')
	retro.data.Integrations.add_custom_path(test_rom_dir)
	assert retro.data.get_file_path('', 'Dekadence-Dekadrive.md', inttype=retro.data.Integrations.CUSTOM_ONLY) == \
		os.path.join(test_rom_dir, 'Dekadence-Dekadrive.md')


def test_custom_path_relative(custom_cleanup):
	assert not retro.data.get_file_path('Airstriker-Genesis', 'rom.md', inttype=retro.data.Integrations.CUSTOM_ONLY)

	retro.data.Integrations.add_custom_path(retro.data.Integrations.STABLE.paths[0])
	assert retro.data.get_file_path('Airstriker-Genesis', 'rom.md', inttype=retro.data.Integrations.CUSTOM_ONLY) == \
		retro.data.get_file_path('Airstriker-Genesis', 'rom.md', inttype=retro.data.Integrations.STABLE)
