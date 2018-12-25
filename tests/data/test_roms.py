from retro.testing import game, handle
import retro.data
import retro.testing.tools


def test_hash(game):
    errors = retro.data.verify_hash(*game)
    handle([], errors)


def test_hash_collisions():
    warnings, errors = retro.testing.tools.verify_hash_collisions()
    handle(warnings, errors)


def test_rom(game):
    warnings, errors = retro.testing.tools.verify_rom(*game)
    handle(warnings, errors)
