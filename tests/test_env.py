import retro
from retro.testing import testenv, handle
import os
import pytest


def test_env_create(testenv):
    json_path = os.path.join(os.path.dirname(__file__), 'dummy.json')
    assert testenv(info=json_path, scenario=json_path)


@pytest.mark.parametrize('obs_type', [retro.Observations.IMAGE, retro.Observations.RAM])
def test_env_basic(obs_type, testenv):
    import gym
    import numpy as np
    json_path = os.path.join(os.path.dirname(__file__), 'dummy.json')
    env = testenv(info=json_path, scenario=json_path, obs_type=obs_type)
    obs = env.reset()
    assert obs.shape == env.observation_space.shape
    obs, rew, done, info = env.step(env.action_space.sample())
    assert obs.shape == env.observation_space.shape
    assert isinstance(rew, float)
    assert rew == 0
    assert isinstance(done, bool)
    assert not done
    assert isinstance(info, dict)


def test_env_data(testenv):
    json_path = os.path.join(os.path.dirname(__file__), 'dummy.json')
    env = testenv(info=json_path, scenario=json_path)
    assert isinstance(env.data[env.system], int)

    env.data['foo'] = 1
    assert env.data['foo'] == 1
    env.reset()
    try:
        a = env.data['foo']
        assert a != 1
    except KeyError:
        pass
