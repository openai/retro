import glob
import hashlib
import json
import os
import re

import retro

whitelist = {
    'Airstriker-Genesis/data.json': {
        'suspicious type >u2 for lives'
    },
    'Qbert-Atari2600/scenario.json': {
        'suspicious variable in done condition: score'
    },
    'ColumnsIII-Genesis/data.json': {
        'suspicious type |u1 for score'
    }
}


def scan_missing():
    missing = []
    for game in retro.list_games():
        gamedir = retro.get_game_path(game)
        if not os.path.isfile(os.path.join(gamedir, 'data.json')):
            missing.append((game, 'data.json'))
        if not os.path.isfile(os.path.join(gamedir, 'scenario.json')):
            missing.append((game, 'scenario.json'))
        if not os.path.isfile(os.path.join(gamedir, 'metadata.json')):
            missing.append((game, 'metadata.json'))
        if not retro.list_states(game):
            missing.append((game, '*.state'))
        if not os.path.isfile(os.path.join(gamedir, 'rom.sha')):
            missing.append((game, 'rom.sha'))
    return missing


def verify_data(game, raw=None):
    file = os.path.join(game, 'data.json')
    try:
        if not raw:
            with open(os.path.join(retro.get_game_path(file))) as f:
                data = json.load(f)
        else:
            data = json.loads(raw)
    except json.JSONDecodeError:
        return [], [(file, 'fail decode')]
    except IOError:
        return [], []

    data = data.get('info')

    warnings = []
    errors = []
    if not data:
        return [], [(file, 'missing info')]
    for variable, definition in data.items():
        if 'address' not in definition:
            errors.append((file, 'missing address for %s' % variable))
        if 'type' not in definition:
            errors.append((file, 'missing type for %s' % variable))
        else:
            if not re.match(r'\|[dinu]1|(>[<=]?|<[>=]?|=[><]?)[dinu][2-8]', definition['type']):
                errors.append((file, 'invalid type %s for %s' % (definition['type'], variable)))
            elif re.match(r'([><=]{2}|=[><]|<[>=]|>[<=])[dinu][2-8]|[><=]{1,2}d[5-8]', definition['type']):
                warnings.append((file, 'suspicious type %s for %s' % (definition['type'], variable)))
    if 'lives' in data and data['lives'].get('type', '') not in ('|u1', '|i1', '|d1'):
        warnings.append((file, 'suspicious type %s for lives' % data['lives']['type']))
    if 'score' in data and (data['score'].get('type', '??')[1:] in ('u1', 'd1', 'n1', 'n2') or 'i' in data['score'].get('type', '')):
        warnings.append((file, 'suspicious type %s for score' % data['score']['type']))

    warnings = [(file, w) for (file, w) in warnings if w not in whitelist.get(file, [])]
    return warnings, errors


def verify_scenario(game, scenario='scenario', raw=None, dataraw=None):
    file = os.path.join(game, '%s.json' % 'scenario')
    try:
        if not raw:
            with open(retro.get_game_path(file)) as f:
                scen = json.load(f)
        else:
            scen = json.loads(raw)
    except json.JSONDecodeError:
        return [], [(file, 'fail decode')]
    except IOError:
        return [], []

    warnings = []
    errors = []
    if 'reward' not in scen or ('variables' not in scen['reward'] and 'script' not in scen['reward']):
        warnings.append((file, 'missing reward'))
    if 'done' not in scen or ('variables' not in scen['done'] and 'script' not in scen['done'] and 'nodes' not in scen['done']):
        warnings.append((file, 'missing done'))

    try:
        if not dataraw:
            datafile = os.path.join(retro.get_game_path(game), 'data.json')
            with open(datafile) as f:
                data = json.load(f)
        else:
            data = json.loads(dataraw)
        data = data.get('info')
        reward = scen.get('reward')
        done = scen.get('done')
        if reward and 'variables' in reward:
            for variable, definition in reward['variables'].items():
                if variable not in data:
                    errors.append((file, 'invalid variable %s' % variable))
                if not definition:
                    errors.append((file, 'invalid definition %s' % variable))
                    continue
                if 'reward' not in definition and 'penalty' not in definition:
                    errors.append((file, 'blank reward %s' % variable))
        if done and 'variables' in done:
            if 'score'in done['variables']:
                warnings.append((file, 'suspicious variable in done condition: score'))
            if 'health' in done['variables'] and 'lives' in done['variables'] and 'condition' not in done:
                warnings.append((file, 'suspicious done condition: health OR lives'))
            if done.get('condition', 'any') == 'all' and (len(done['variables']) + len(done.get('nodes', {}))) < 2:
                errors.append((file, 'incorrect done condition all with only 1 check'))
            if done.get('condition', 'any') == 'any' and (len(done['variables']) + len(done.get('nodes', {}))) > 2:
                warnings.append((file, 'suspicious done condition any with more than 2 checks'))
            for variable, definition in done['variables'].items():
                if 'op' not in definition:
                    errors.append((file, 'invalid done condition %s' % variable))
                elif definition.get('reference', 0) == 0:
                    if 'op' in ('equal', 'negative-equal'):
                        warnings.append((file, 'incorrect op: zero for %s' % variable))
                    elif 'op' == 'not-equal':
                        warnings.append((file, 'incorrect op: nonzero for %s' % variable))
                    elif 'op' == 'less-than':
                        warnings.append((file, 'incorrect op: negative for %s' % variable))
                    elif 'op' == 'greater-than':
                        warnings.append((file, 'incorrect op: positive for %s' % variable))
                if data:
                    if variable not in data:
                        errors.append((file, 'invalid variable %s' % variable))
                    else:
                        if 'i' not in data[variable].get('type', '') and definition.get('op', '') == 'negative' and definition.get('measurement') != 'delta':
                            errors.append((file, 'op: negative on unsigned %s' % variable))
    except (json.JSONDecodeError, IOError):
        pass

    warnings = [(file, w) for (file, w) in warnings if w not in whitelist.get(file, [])]
    return warnings, errors


def verify_default_state(game, raw=None):
    file = os.path.join(game, 'metadata.json')
    try:
        if not raw:
            with open(retro.get_game_path(file)) as f:
                metadata = json.load(f)
        else:
            metadata = json.loads(raw)
    except json.JSONDecodeError:
        return [], [(file, 'fail decode')]
    except IOError:
        return [], []

    errors = []
    state = metadata.get('default_state')
    if not state:
        return [], [(file, 'default state missing')]
    if state not in retro.list_states(game):
        errors.append((file, 'invalid default state %s' % state))

    return [], errors


def verify_json():
    warnings = []
    errors = []
    for game in retro.list_games():
        gamedir = retro.get_game_path(game)
        w, e = verify_data(game)
        warnings.extend(w)
        errors.extend(e)

        w, e = verify_scenario(game)
        warnings.extend(w)
        errors.extend(e)
    return warnings, errors


def verify_hash(game):
    errors = []
    gamedir = retro.get_game_path(game)
    rom = retro.get_romfile_path(game)
    system = retro.get_romfile_system(rom)
    with open(os.path.join(gamedir, 'rom.sha')) as f:
        expected_shas = f.read().strip().split('\n')
    with open(rom, 'rb') as f:
        if system == 'Nes':
            # Chop off header for checksum
            f.read(16)
        real_sha = hashlib.sha1(f.read()).hexdigest()
    if real_sha not in expected_shas:
        errors.append((game, 'sha mismatch'))
    return [], errors


def verify_hash_collisions():
    errors = []
    seen_hashes = {}
    for game in retro.list_games():
        gamedir = retro.get_game_path(game)
        try:
            with open(os.path.join(gamedir, 'rom.sha')) as f:
                expected_shas = f.read().strip().split('\n')
        except IOError:
            continue
        for expected_sha in expected_shas:
            seen = seen_hashes.get(expected_sha, [])
            seen.append(game)
            seen_hashes[expected_sha] = seen
    for sha, games in seen_hashes.items():
        if len(games) < 2:
            continue
        for game in games:
            errors.append((game, 'sha duplicate'))
    return [], errors


def verify_genesis(game):
    warnings = []
    errors = []
    whitelist = {
    }
    rom = retro.get_romfile_path(game)
    if not rom.endswith('.md'):
        errors.append((game, 'invalid extension for %s' % rom))
    if game in whitelist:
        return [], []
    with open(rom, 'rb') as f:
        header = f.read(768)
    if header[0x100:0x105] not in (b'SEGA ', b' SEGA'):
        errors.append((game, 'invalid genesis rom'))
    return warnings, errors


def verify_rom(game):
    try:
        rom = retro.get_romfile_path(game)
    except FileNotFoundError:
        return [], [(game, 'ROM file missing')]

    if game.endswith('-Genesis'):
        return verify_genesis(game)

    return [], []


def verify_roms():
    warnings = []
    errors = []

    for game in retro.list_games():
        w, e = verify_rom(game)
        warnings.extend(w)
        errors.extend(e)

    return warnings, errors


def main():
    missing = scan_missing()
    print('Files missing:')
    for game, file in missing:
        print('%s: %s' % (game, file))

    warnings = []
    errors = []

    w, e = verify_json()
    warnings.extend(w)
    errors.extend(e)

    w, e = verify_hash()
    warnings.extend(w)
    errors.extend(e)

    w, e = verify_roms()
    warnings.extend(w)
    errors.extend(e)

    print()
    print('File errors:')
    for file, error in errors:
        print('%s: %s' % (file, error))

    print()
    print('File warnings:')
    for file, warning in warnings:
        print('%s: %s' % (file, warning))

    return not errors


if __name__ == '__main__':
    if not main():
        exit(1)
