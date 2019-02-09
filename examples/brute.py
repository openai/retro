"""
Implementation of The Brute from "Revisiting the Arcade Learning Environment:
Evaluation Protocols and Open Problems for General Agents" by Machado et al.
https://arxiv.org/abs/1709.06009

This is an agent that uses the determinism of the environment in order to do
pretty well at a number of retro games.  It does not save emulator state but
does rely on the same sequence of actions producing the same result when played
back.

This script depends on baselines
"""

# TODO:
# save best bk2 periodically by walking agressively through tree

import random
import time
import os

import gin
import numpy as np
import retro
from baselines import logger
from baselines.common.retro_wrappers import TimeLimit
import gym


EXPLORATION_PARAM = 0.005


class Frameskip(gym.Wrapper):
    def __init__(self, env, skip=4):
        gym.Wrapper.__init__(self, env)
        self._skip = skip

    def reset(self):
        return self.env.reset()

    def step(self, act):
        total_reward = 0.0
        done = None
        for i in range(self._skip):
            obs, reward, done, info = self.env.step(act)
            total_reward += reward
            if done:
                break

        return obs, total_reward, done, info


class Node:
    def __init__(self, value=-np.inf, children=None):
        self.value = value
        self.visits = 0
        self.children = {} if children is None else children

    def __repr__(self):
        return "<Node value=%f visits=%d len(children)=%d>" % (
            self.value,
            self.visits,
            len(self.children),
        )


def format_seconds(s):
    """Format seconds in some more human readable but less accurate form"""
    s = int(s)
    w, d, h, m, s = (
        s // (60 * 60 * 24 * 7),
        s // (60 * 60 * 24) % 7,
        s // (60 * 60) % 24,
        s // 60 % 60,
        s % 60,
    )
    parts = []
    for value, suffix in [(w, "w"), (d, "d"), (h, "h"), (m, "m"), (s, "s")]:
        if len(parts) > 0 or (len(parts) == 0 and value > 0):
            parts.append(f"{value}{suffix}")

    if len(parts) > 0:
        return " ".join(parts[:2])

    return f"{s}s"


def select_actions(root, action_space, max_episode_steps):
    """
    Select actions from the tree

    Normally we select the greedy action that has the highest reward
    associated with that subtree.  We have a small chance to select a
    random action based on the exploration param and visit count of the
    current node at each step.
    """
    node = root
    acts = []

    steps = 0
    while steps < max_episode_steps:
        if node is None:
            act = action_space.sample()
        else:
            epsilon = EXPLORATION_PARAM / np.log(node.visits + 2)
            if random.random() < epsilon:
                # random action
                act = action_space.sample()
            else:
                # greedy action
                act_value = {}
                for act in range(action_space.n):
                    if node is not None and act in node.children:
                        act_value[act] = node.children[act].value
                    else:
                        act_value[act] = -np.inf
                best_value = max(act_value.values())
                best_acts = [
                    act for act, value in act_value.items() if value == best_value
                ]
                act = random.choice(best_acts)

            if act in node.children:
                node = node.children[act]
            else:
                node = None

        acts.append(act)
        steps += 1

    return acts


def rollout(env, acts):
    """
    Perform a rollout using a preset collection of actions
    """
    total_rew = 0
    env.reset()
    steps = 0
    for act in acts:
        _obs, rew, done, _info = env.step(act)
        steps += 1
        total_rew += rew
        if done:
            break

    return steps, total_rew


def update_tree(root, executed_acts, total_rew):
    """
    Given the tree, a list of actions that were executed before the game ended, and a reward, update the tree
    so that the path formed by the executed actions are all updated to the new reward.
    """
    root.value = max(total_rew, root.value)
    root.visits += 1
    new_nodes = 0

    node = root
    for step, act in enumerate(executed_acts):
        if act not in node.children:
            node.children[act] = Node()
            new_nodes += 1
        node = node.children[act]
        node.value = max(total_rew, node.value)
        node.visits += 1

    return new_nodes


class Brute:
    def __init__(self, env, max_episode_steps):
        self.node_count = 1
        self._root = Node()
        self._env = env
        self._action_space = self.env.action_space
        self._max_episode_steps = max_episode_steps

    def run(self):
        acts = select_actions(self._root, self._action_space, self._max_episode_steps)
        steps, total_rew = rollout(self._env, acts)
        executed_acts = acts[:steps]
        self.node_count += update_tree(self._root, executed_acts, total_rew)
        return executed_acts, total_rew


def test_brute():
    from csh_trees.envs import create_maze_mdp

    def make_env():
        env = create_maze_mdp()
        env = TimeLimit(env, max_episode_steps=100)
        return env

    env = make_env()
    b = Brute(env=env, max_episode_steps=100)
    acts, total_rew = b.run()
    assert len(acts) > 1

    read_acts = []
    node = b.root
    while len(node.children) > 0:
        assert len(node.children) == 1
        node_values = [
            (child.value, act, child) for act, child in node.children.items()
        ]
        _next_value, act, next_node = sorted(node_values)[-1]
        if next_node.visits == 0:
            break
        read_acts.append(act)
        node = next_node
    assert acts == read_acts
    assert b.root.value == total_rew

    # make sure we get the same reward when we do the rollout ourselves
    empirical_rew = 0
    b.env.reset()
    for act in acts:
        _obs, rew, done, info = b.env.step(act)
        empirical_rew += rew
        if done:
            break
    assert empirical_rew == total_rew


@gin.configurable
def brute_retro(
    game,
    max_episode_steps=4500,
    timestep_limit=100_000_000,
    log_interval=100,
    state=retro.State.DEFAULT,
    scenario=None,
):
    env = retro.make(game, state, use_restricted_actions=retro.Actions.DISCRETE)
    env = Frameskip(env)
    env = TimeLimit(env, max_episode_steps=max_episode_steps)

    start = time.time()
    b = Brute(env, max_episode_steps=max_episode_steps)
    timesteps = 0
    loop_count = 0
    best_rew = 0
    while True:
        acts, rew = b.run()

        logger.logkv_mean("avg_ep_len", len(acts))
        logger.logkv_mean("avg_ep_rew", rew)

        if rew > best_rew:
            logger.log(f"new best reward {rew} => {best_rew}")
            best_bk2_path = os.path.join(logger.get_dir(), "best.bk2")
            env.unwrapped.record_movie(best_bk2_path)
            env.reset()
            for act in acts:
                env.step(act)
            env.unwrapped.stop_record()

        if timesteps > timestep_limit:
            logger.log("timestep limit exceeded")
            break

        if loop_count == 0 or loop_count % log_interval == 0:
            logger.logkvs(
                loop_count=loop_count,
                timesteps=timesteps,
                timestep_limit=timestep_limit,
                percent=timesteps / timestep_limit * 100,
                timesteps_per_sec=(timesteps / (time.time() - start)),
                node_count=b.node_count,
            )
            logger.dumpkvs()
            elapsed = format_seconds(time.time() - start)
            remaining = format_seconds(
                (time.time() - start) / timesteps * (timestep_limit - timesteps)
            )
            logger.info(f"elapsed {elapsed}  remaining {remaining}")
        loop_count += 1


if __name__ == "__main__":
    brute_retro(game="Airstriker-Genesis", timestep_limit=1e9)
