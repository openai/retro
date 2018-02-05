from retro.testing import game, handle
import retro.testing.tools


def test_hash(game):
    warnings, errors = retro.testing.tools.verify_hash(game)
    handle(warnings, errors)


def test_hash_collisions():
    warnings, errors = retro.testing.tools.verify_hash_collisions()
    handle(warnings, errors)


def test_rom(game):
    warnings, errors = retro.testing.tools.verify_rom(game)
    handle(warnings, errors)
