from retro.testing import game, handle
import retro.testing.tools


def test_data(game):
    warnings, errors = retro.testing.tools.verify_data(*game)
    handle(warnings, errors)


def test_scenario(game):
    warnings, errors = retro.testing.tools.verify_scenario(*game)
    handle(warnings, errors)


def test_missing():
    missing = retro.testing.tools.scan_missing()
    handle([(game, 'missing %s' % file) for (game, file) in missing], [])


def test_default_states(game):
    warnings, errors = retro.testing.tools.verify_default_state(*game)
    handle(warnings, errors)
