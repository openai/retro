import glob
import hashlib
import json
import os
import re

import retro.data


def load_whitelist(game, inttype):
    try:
        with open(retro.data.get_file_path(game, 'metadata.json', inttype | retro.data.Integrations.STABLE)) as f:
            whitelist = json.load(f).get('whitelist', {})
    except json.JSONDecodeError:
        return None, [(metadata_file, 'fail decode')]
    except IOError:
        return None, [(metadata_file, 'fail I/O')]
    return whitelist, []


def scan_missing():
    missing = []
    for game in retro.data.list_games(retro.data.Integrations.ALL):
        if not retro.data.get_file_path(game, 'data.json', retro.data.Integrations.ALL):
            missing.append((game, 'data.json'))
        if not retro.data.get_file_path(game, 'scenario.json', retro.data.Integrations.ALL):
            missing.append((game, 'scenario.json'))
        if not retro.data.get_file_path(game, 'metadata.json', retro.data.Integrations.ALL):
            missing.append((game, 'metadata.json'))
        if not retro.data.list_states(game, retro.data.Integrations.ALL):
            missing.append((game, '*.state'))
        if not retro.data.get_file_path(game, 'rom.sha', retro.data.Integrations.ALL):
            missing.append((game, 'rom.sha'))
    return missing


def verify_data(game, inttype, raw=None):
    file = os.path.join(str(inttype), game, 'data.json')
    path = retro.data.get_file_path(game, 'data.json', inttype)
    if not path:
        return [], []
    try:
        if not raw:
            with open(path) as f:
                data = json.load(f)
        else:
            data = json.loads(raw)
    except json.JSONDecodeError:
        return [], [(file, 'fail decode')]
    except IOError:
        return [], [(file, 'fail I/O')]

    whitelist, errors = load_whitelist(game, inttype)
    if errors:
        return [], errors
    warnings = []

    data = data.get('info')
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

    whitelist = {(file, w) for w in whitelist.get('data.json', [])}
    all_warnings = {(file, w) for (file, w) in warnings}
    warnings = list(all_warnings - whitelist)
    errors.extend(('metadata.json', 'missing warning "%s: %s"' % (file, w)) for (file, w) in whitelist - all_warnings)
    return warnings, errors


def verify_scenario(game, inttype, scenario='scenario', raw=None, dataraw=None):
    file = os.path.join(str(inttype), game, '%s.json' % scenario)
    path = retro.data.get_file_path(game, '%s.json' % scenario, inttype)
    if not path:
        return [], []
    try:
        if not raw:
            with open(path) as f:
                scen = json.load(f)
        else:
            scen = json.loads(raw)
    except json.JSONDecodeError:
        return [], [(file, 'fail decode')]
    except IOError:
        return [], [(file, 'fail I/O')]

    whitelist, errors = load_whitelist(game, inttype)
    if errors:
        return [], errors
    warnings = []
    if 'rewards' in scen:
        for i, r in enumerate(scen['rewards']):
            if 'variables' not in r and 'script' not in r:
                warnings.append((file, 'missing reward in rewards[%d]' % i))
            elif 'variables' in r and 'script' in r:
                warnings.append((file, 'both variables and script present in rewards[%d]' % i))
        if 'reward' in scen:
            warnings.append((file, 'reward and rewards both present'))
    elif 'reward' not in scen or ('variables' not in scen['reward'] and 'script' not in scen['reward']):
        warnings.append((file, 'missing reward'))
    elif 'variables' in scen['reward'] and 'script' in scen['reward']:
        warnings.append((file, 'both variables and script present in reward'))

    if 'done' not in scen or ('variables' not in scen['done'] and 'script' not in scen['done'] and 'nodes' not in scen['done']):
        warnings.append((file, 'missing done'))

    try:
        if not dataraw:
            datafile = retro.data.get_file_path(game, 'data.json', inttype=inttype | retro.data.Integrations.STABLE)
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

    whitelist = {(file, w) for w in whitelist.get(os.path.split(file)[-1], [])}
    all_warnings = {(file, w) for (file, w) in warnings}
    warnings = list(all_warnings - whitelist)
    errors.extend(('metadata.json', 'missing warning "%s: %s"' % (file, w)) for (file, w) in whitelist - all_warnings)
    return warnings, errors


def verify_default_state(game, inttype, raw=None):
    file = os.path.join(str(inttype), game, 'metadata.json')
    path = retro.data.get_file_path(game, 'metadata.json', inttype)
    if not path:
        return [], []
    try:
        if not raw:
            with open(path) as f:
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
    if state not in retro.data.list_states(game, inttype | retro.data.Integrations.STABLE):
        errors.append((file, 'invalid default state %s' % state))

    return [], errors


def verify_hash_collisions():
    errors = []
    seen_hashes = {}
    for game in retro.data.list_games(retro.data.Integrations.ALL):
        shafile = retro.data.get_file_path(game, 'rom.sha', retro.data.Integrations.ALL)
        try:
            with open(os.path.join(shafile, 'rom.sha')) as f:
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


def verify_genesis(game, inttype):
    whitelist, errors = load_whitelist(game, inttype)
    if errors:
        return [], errors
    warnings = []

    rom = retro.data.get_romfile_path(game, inttype=inttype)
    if not rom.endswith('.md'):
        errors.append((game, 'invalid extension for %s' % rom))
    if 'rom.md' in whitelist:
        return [], []
    with open(rom, 'rb') as f:
        header = f.read(512)
    if header[0x100:0x105] not in (b'SEGA ', b' SEGA'):
        errors.append((game, 'invalid genesis rom'))
    return warnings, errors


def verify_extension(game, inttype):
    whitelist, errors = load_whitelist(game, inttype)
    if errors:
        return [], errors
    warnings = []

    rom = os.path.split(retro.data.get_romfile_path(game, inttype=inttype))[-1]
    platform = retro.data.EMU_EXTENSIONS.get(os.path.splitext(rom)[-1])

    if not platform or not game.endswith('-%s' % platform):
        errors.append((game, 'invalid extension for %s' % rom))
    if rom in whitelist:
        return [], []
    return warnings, errors


def verify_rom(game, inttype):
    try:
        rom = retro.data.get_romfile_path(game, inttype=inttype)
    except FileNotFoundError:
        return [], [(game, 'ROM file missing')]

    if game.endswith('-Genesis'):
        return verify_genesis(game, inttype)
    return verify_extension(game, inttype)
