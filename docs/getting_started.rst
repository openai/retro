Getting Started
=====================================

Gym Retro requires one of the supported versions of Python (3.5, 3.6, or 3.7). Please make sure to install the appropriate distribution for your OS beforehand. Please note that due to compatibility issues with some of the cores, 32-bit operating systems are not supported.

.. code-block:: shell

    pip3 install gym-retro

See the section :ref:`development` if you want to build Gym Retro yourself (this is only useful if you want to change the C++ code, not required to integrate new games).

Create a Gym Environment
--------------------------------------

After installing you can now create a `Gym <https://gym.openai.com/>`_ environment in Python:

.. code-block:: python

    import retro
    env = retro.make(game='Airstriker-Genesis')

``Airstriker-Genesis`` has a non-commercial ROM that is included by default.

Please note that other ROMs are not included and you must obtain them yourself.  Most ROM hashes are sourced from their respective No-Intro SHA-1 sums.  See :ref:`importing-roms` for information about importing ROMs into Gym Retro.

Example Usage
--------------------------------------

Gym Retro is useful primarily as a means to train RL on classic video games, though it can also be used to control those video games from Python.

Here are some example ways to use Gym Retro:

Interactive Script
~~~~~~~~~~~~~~~~~~~~~~

There is a Python script that lets you interact with the game using the Gym interface.  Run it like this:

.. code-block:: shell

    python3 -m retro.examples.interactive --game Airstriker-Genesis

You can use the arrow keys and the ``X`` key to control your ship and fire.  This Python script lets you try out an environment using only the Gym Retro Python API and is quite basic.  For a more advanced tool, check out the :ref:`integration-ui`.

Random Agent
~~~~~~~~~~~~~~~~~~~~~~

A random agent that chooses a random action on each timestep looks much like the example random agent for `Gym <https://gym.openai.com/>`_:

.. literalinclude:: ../retro/examples/trivial_random_agent.py

A more full-featured random agent script is available in the examples dir:

.. code-block:: shell

    python3 -m retro.examples.random_agent --game Airstriker-Genesis

It will print the current reward and will exit when the scenario is done. Note that it will throw an exception if no reward or scenario data is defined for that game. This script is useful to see if a scenario is properly set up and that the reward function isn't too generous.

Brute
~~~~~~~~~~~~~~~~~~~~~~

There is a simple but effective reinforcement learning algorithm called "the Brute" from `"Revisiting the Arcade Learning Environment" <https://arxiv.org/abs/1709.06009>`_  by Machado et al. which works on deterministic environments like Gym Retro games and is easy to implement.  To run the example:

.. code-block:: shell

    python3 -m retro.examples.brute --game Airstriker-Genesis

This algorithm works by building up a sequence of button presses that do well in the game, it doesn't look at the screen at all.  It will print out the best reward seen so far while training.

PPO
~~~~~~~~~~~~~~~~~~~~~~

Using `"Proximal Policy Optimization" <https://arxiv.org/abs/1707.06347>`_ by Schulman et al., you can train an agent to play many of the games, though it takes awhile and is much faster with a GPU.

This example requires installing `OpenAI Baselines <https://github.com/openai/baselines>`_.  Once installed, you can run it:

.. code-block:: shell

    python3 -m retro.examples.ppo --game Airstriker-Genesis


This will take awhile to train, but will print out progress as it goes.  More information about PPO can be found in `Spinning Up <https://spinningup.openai.com/en/latest/algorithms/ppo.html>`_.


Integrations
--------------------------------------

What games have already been integrated?  Note that this will display all defined environments, even ones for which ROMs are missing.

.. code-block:: python

    import retro
    retro.data.list_games()

The actual integration data can be see in the `Gym Retro Github repo <https://github.com/openai/retro/tree/master/retro/data/stable>`_.

.. _importing-roms:

Importing ROMs
--------------------------------------

If you have the correct ROMs on your computer (identified by the `rom.sha` file for each game integration), you can import them using the import script:

.. code-block:: shell

    python3 -m retro.import /path/to/your/ROMs/directory/

This will copy all matching ROMs to their corresponding Gym Retro game integration directories.

Your ROMs must be in the :ref:`supported-roms` list and must already have an integration.  To add a ROM yourself, check out :ref:`game-integration`.

Many ROMs should be available from the `No-Intro Collection on Archive.org <https://archive.org/details/No-Intro-Collection_2016-01-03_Fixed>`_ and the import script will search inside of zip files.