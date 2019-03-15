Python API
=====================================

RetroEnv
--------------------------------------

The Python API consists primarily of :func:`retro.make`, :class:`retro.RetroEnv`, and a few enums.  The main function most users will want is :func:`retro.make`.

.. autofunction:: retro.make

.. autoclass:: retro.RetroEnv

If you want to specify either the default state named in the game integration's `metadata.json` or specify that you want to start from the initial power on state of the console, you can use the :class:`retro.State` enum:

.. autoclass:: retro.State
   :members:

Actions
--------------------------------------

There are a few possible action spaces included with :class:`retro.RetroEnv`:

.. autoclass:: retro.Actions
   :members:

You can also create your own action spaces derived from these.  For an example, see `discretizer.py <https://github.com/openai/retro/blob/master/retro/examples/discretizer.py>`_.  This file shows how to use ``retro.Actions.Discrete`` as well as how to make a custom wrapper that reduces the action space from ``126`` actions to ``7``

Observations
--------------------------------------

The default observations are RGB images of the game, but you can view RAM values instead (often much smaller than the RGB images and also your agent can observate the game state more directly).  If you want variable values, any variables defined in ``data.json`` will appear in the ``info`` dict after each step.

.. autoclass:: retro.Observations
   :members:

Multiplayer Environments
--------------------------------------

A small number of games support multiplayer.  To use this feature, pass ``players=<n>`` to :class:`retro.RetroEnv`.  Here is an example random agent that controls both paddles in ``Pong-Atari2600``:

.. literalinclude:: ../retro/examples/trivial_random_agent_multiplayer.py


Replay files
--------------------------------------

Gym Retro can create  `.bk2
<http://tasvideos.org/Bizhawk/BK2Format.html>`_ files which are recordings of an initial game state and a series of button presses.  Because the emulators are deterministic, you will see the same output each time you play back this file.  Because it only stores button presses, the file can be about 1000 times smaller than storing the full video.

In addition, if you wish to use the stored button presses for training, they may be useful.  For example, there are `replay files for each Sonic The Hedgehog level <https://github.com/openai/retro-movies>`_ that were made available for the `Gym Retro Contest <https://openai.com/blog/retro-contest/>`_.

You can create and view replay files using the :ref:`integration-ui` (Game > Play Movie...).  If you want to use replay files from Python, see the following sections.

Record
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you have an agent playing a game, you can record the gameplay to a ``.bk2`` file for later processing:

.. code-block:: python

    import retro

    env = retro.make(game='Airstriker-Genesis', record='.')
    env.reset()
    while True:
        _obs, _rew, done, _info = env.step(env.action_space.sample())
        if done:
            break

Playback
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Given a ``.bk2`` file you can load it in python and either play it back or use the actions for training.

.. code-block:: python

    import retro

    movie = retro.Movie('Airstriker-Genesis-Level1-000000.bk2')
    movie.step()

    env = retro.make(
        game=movie.get_game(),
        state=None,
        # bk2s can contain any button presses, so allow everything
        use_restricted_actions=retro.Actions.ALL,
        players=movie.players,
    )
    env.initial_state = movie.get_state()
    env.reset()

    while movie.step():
        keys = []
        for p in range(movie.players):
            for i in range(env.num_buttons):
                keys.append(movie.get_key(i, p))
        env.step(keys)


Render to Video
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This requires `ffmpeg <https://www.ffmpeg.org/>`_ to be installed and writes the output to the directory that the input file is located in.

.. code-block:: shell

    python3 -m retro.scripts.playback_movie Airstriker-Genesis-Level1-000000.bk2
