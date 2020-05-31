import os

import retro
from baselines.bench import Monitor
from baselines.common.atari_wrappers import (WarpFrame, FrameStack, ScaledFloatFrame, MaxAndSkipEnv)
from baselines.common.retro_wrappers import (MovieRecord, RewardScaler)
from baselines.common.vec_env import SubprocVecEnv, DummyVecEnv
from baselines.common.wrappers import TimeLimit
from baselines.logger import configure
from baselines.ppo2 import ppo2


def wrap_deepmind_n64(env,
                      reward_scale=1 / 100.0,
                      frame_stack=4,
                      frame_skip=4,
                      width=150,
                      height=100):
    env = MaxAndSkipEnv(env, skip=frame_skip)
    env = WarpFrame(env, width=width, height=height)
    env = FrameStack(env, frame_stack)
    env = ScaledFloatFrame(env)
    env = RewardScaler(env, scale=1 / 100.0)
    return env


def wrap_monitoring_n64(env,
                        max_episode_steps=5000,
                        monitor_filepath=None,
                        movie_dir=None,
                        record_movie_every=10):
    env = TimeLimit(env, max_episode_steps=max_episode_steps)
    if monitor_filepath is not None:
        env = Monitor(env, monitor_filepath, allow_early_resets=True)
    if movie_dir is not None:
        env = MovieRecord(env, movie_dir, k=record_movie_every)
    return env


def main():
    expdir = os.path.join("/home/wulfebw/experiments", "ssb64_001")
    os.makedirs(expdir, exist_ok=True)
    monitor_filepath = os.path.join(expdir, "monitor.csv")
    movie_dir = os.path.join(expdir, "movies")
    os.makedirs(movie_dir, exist_ok=True)

    configure(dir=expdir)
    
    def make_env():
        retro.data.add_custom_integration("custom")
        env = retro.n64_env.N64Env(game="SuperSmashBros-N64",
                                   use_restricted_actions=retro.Actions.MULTI_DISCRETE,
                                   inttype=retro.data.Integrations.CUSTOM,
                                   obs_type=retro.Observations.IMAGE)
        env = wrap_deepmind_n64(env)
        env = wrap_monitoring_n64(env, monitor_filepath=monitor_filepath, movie_dir=movie_dir)
        return env

    venv = SubprocVecEnv([make_env] * 4)
    # venv = DummyVecEnv([make_env] * 1)
    ppo2.learn(
        network='impala_cnn',
        env=venv,
        total_timesteps=int(100e6),
        nsteps=128,
        nminibatches=2,
        lam=0.95,
        gamma=0.99,
        noptepochs=3,
        log_interval=1,
        ent_coef=.01,
        lr=lambda f: f * 2.5e-4,
        cliprange=0.1,
        save_interval=10
    )


if __name__ == '__main__':
    main()
