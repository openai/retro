import os

from baselines.bench import Monitor
from baselines.common.atari_wrappers import (WarpFrame, FrameStack, ScaledFloatFrame, MaxAndSkipEnv)
from baselines.common.retro_wrappers import (MovieRecord, RewardScaler)
from baselines.common.vec_env import (SubprocVecEnv, DummyVecEnv, VecFrameStack)
from baselines.common.wrappers import TimeLimit
from baselines.logger import configure
from baselines.ppo2 import ppo2
import retro
import tensorflow as tf


def wrap_n64(env, reward_scale=1 / 100.0, frame_skip=4, width=150, height=100):
    env = MaxAndSkipEnv(env, skip=frame_skip)
    env = WarpFrame(env, width=width, height=height)
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
    expdir = os.path.join("/home/wulfebw/experiments", "ssb64_002", "run_007")
    os.makedirs(expdir, exist_ok=True)
    monitor_filepath = os.path.join(expdir, "monitor.csv")
    movie_dir = os.path.join(expdir, "movies")
    os.makedirs(movie_dir, exist_ok=True)
    load_filepath = "/home/wulfebw/experiments/ssb64_002/run_006/checkpoints/00250"

    # This configures baselines logging.
    configure(dir=expdir)
    # Creating the session here prevents tf from using all the gpu memory, which
    # causes a failure in the emulator. I'm not sure why because when the emulator
    # is running with angrylion I thought it wasn't using any gpu memory, but
    # there's a lot here I don't understand so oh well.
    # gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.9)
    gpu_options = tf.GPUOptions(allow_growth=True)
    sess = tf.Session(config=tf.ConfigProto(allow_soft_placement=True,
                                            intra_op_parallelism_threads=1,
                                            inter_op_parallelism_threads=1,
                                            gpu_options=gpu_options))

    def make_env(rank):
        retro.data.add_custom_integration("custom")
        env = retro.n64_env.N64Env(game="SuperSmashBros-N64",
                                   use_restricted_actions=retro.Actions.MULTI_DISCRETE,
                                   inttype=retro.data.Integrations.CUSTOM,
                                   obs_type=retro.Observations.IMAGE)
        env = wrap_n64(env)
        env = wrap_monitoring_n64(env,
                                  monitor_filepath=monitor_filepath,
                                  movie_dir=movie_dir)
        return env

    def make_vec_env(nenvs=4, frame_stack=4):
        venv = SubprocVecEnv([lambda: make_env(rank) for rank in range(nenvs)])
        # Uncomment this line in place of the one above for debugging.
        # venv = DummyVecEnv([lambda: make_env(0)])

        # Perform the frame stack at the vectorized environment level as opposed to at
        # the individual environment level. I think this allows you to communicate fewer
        # images across processes.
        venv = VecFrameStack(venv, frame_stack)
        return venv

    venv = make_vec_env(nenvs=8)
    ppo2.learn(network='impala_cnn',
               env=venv,
               total_timesteps=int(100e6),
               nsteps=256,
               nminibatches=8,
               lam=0.95,
               gamma=0.999,
               noptepochs=4,
               log_interval=1,
               ent_coef=.01,
               lr=lambda f: f * 2e-4,
               cliprange=0.1,
               save_interval=10,
               load_path=load_filepath)


if __name__ == '__main__':
    main()
