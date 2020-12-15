.. _game-integration:

Game Integration
=====================================

Integrating a game means taking a video game ROM file and setting it up as a reinforcement learning environment by defining 3 things:

* A starting state
* A reward function
* A done condition

Once integrated, you will be able to use the game through the Gym Retro Python API as a `Gym <https://gym.openai.com/>`_ environment.

If you are going to integrate a new game, you'll need a ROM for the correct system, see :ref:`supported-roms` for a list.

Example Integration
--------------------------------------

This is a list of the integration files for the game `Airstriker-Genesis <https://github.com/openai/retro/blob/master/retro/data/stable/Airstriker-Genesis>`_.

``Level1.state``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is a savestate from the beginning of the game, restarting the environment will put the agent at this point in the game.

``data.json``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This file defines the list of game-related variables that python can see based on their memory addresses in the games

.. literalinclude:: ../retro/data/stable/Airstriker-Genesis/data.json

``scenario.json``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This file defines the reward function and done condition using the variables defined in `data.json`

.. literalinclude:: ../retro/data/stable/Airstriker-Genesis/scenario.json

``metadata.json``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This file defines the default starting state if no state is specified by the user as well as some miscellaneous debugging information. 

.. literalinclude:: ../retro/data/stable/Airstriker-Genesis/metadata.json

``rom.md``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is the ROM file used for this game, with a few exceptions, ROM files are not included in Gym Retro, but will be in your local copy of Gym Retro after you import them.

``rom.sha``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is the SHA1 hash of the `rom.md` file, used for importing ROMs.

.. literalinclude:: ../retro/data/stable/Airstriker-Genesis/rom.sha

These are all the files used in an integration.  The next section will describe the files in more detail.

Integration Files
--------------------------------------

States
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Emulation allows the entire state of a video game system to be stored to disk and restored. These files are specific to the emulator, but always end with `.state`. These are identical to the versions used in the standalone versions of the emulators but gzipped.

Variable Locations ``data.json``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Information about the inner workings of games are stored alongside the ROM in a file named ``data.json``. This JSON file documents "ground truth" information about a game, including the locations and formats of variables in memory. These manifests are separated into sections, although only one section currently is defined:

The ``info`` section of the manifest lists game variables' memory addresses. Each entry in the ``info`` section consists of a key naming the memory address and the following values:

- ``address``: The address into a RAM array of the first byte of the variable.
- ``type``: A type descriptor for this variable. See the above addendum for the format of this value.

The following manifest shows an example of a game that has one variable, ``score``, located at byte 128 that is 4 bytes wide in unsigned big endian format:

.. code-block:: json

	{
		"info": {
			"score": {
				"address": 128,
				"type": ">u4"
			}
		}
	}

For more information on the possible variable types, see :ref:`appendix-types`.

Scenario ``scenario.json``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Information pertaining to reward functions and done conditions can either be specified by manually overriding functions in :class:`retro.RetroEnv` or can be done by writing a scenario file.

Scenario files contain information that is used to compute the reward function and done condition from variables defined in the information manifest. Each variable specified in the scenario file is multiplied by a ``reward`` value if positive and a ``penalty`` value if negative and then summed up to create the reward for that step. Similarly, states of these variables can be checked to see if the game is over. By default the scenario file will be loaded from ``scenario.json``, but alternative scenario files can be specified in the :class:`retro.RetroEnv` constructor.

Scenario files are again JSON and specified with the following sections:

The ``reward`` section used to calculate the reward function, and it split into the following subsections:

The ``variables`` subsection is used for defining how to calculate the reward function from the current state of memory. For each variable in the ``variables`` section, a value is calculated, multiplied by a coefficient, then added to the reward function for this step. How a value is extracted is specified by the ``op``/``measurement``/``reference`` values (see the addendum below on operations for the meanings of these). The default ``measurement`` is ``delta``. There is no default ``op``, and by default the value is passed through raw.

- ``reward``: A coefficient multiplied by the value when the value is positive.
- ``penalty``: A coefficient multiplied by the value when the value is negative.

A negative ``penalty`` would imply addition to the reward function instead of subtraction as the value to be multiplied by the coefficient is negative.

The ``time`` subsection is used for creating rewards based off of how many steps are taken. Two values can be specified:

- ``reward``: A value to be added to the reward function every step.
- ``penalty``: A value to be subtracted from the reward function every step.

The ``done`` section is used to calculate if the end of a game has been reached. At the top level the following property is available:

- ``condition``: Specifies how the ``done`` conditions should be combined
  - ``any``: Any of the conditions in the ``done`` section is fulfilled. This is the default.
  - ``all``: All of the conditions in the ``done`` section are fulfilled.

Currently it has one subsection:

The ``variables`` subsection specifies how to calculate the done condition from the current state of memory. Each variable in the ``variables`` subsection is extracted per the ``op``/``measurement``/``reference`` values (see the addendum below on operations for the meanings of these). The default ``measurement`` is ``absolute``. There is no default ``op``, and by default the value is ignored.

For more information on the conditions that can be defined, see :ref:`appendix-operations`.

.. _integration-ui:

The Integration UI
--------------------------------------

The integration UI helps you easily find variables and see what is going on with the reward function.  You can download the compiled UI package for your platform here:

* `Windows Integration UI <https:///storage.googleapis.com/gym-retro/builds/Gym%20Retro-latest-win64.zip>`_
* `Mac Integration UI <https:///storage.googleapis.com/gym-retro/builds/Gym%20Retro-latest-Darwin.dmg>`_

Integrating a new ROM
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Open the Gym Retro Integration UI
2. Load a new game — ``Command-Shift-O`` on Mac
3. Select the ROM of the game you'd like to integrate in the menu
4. Name the game
5. The game will open. To see what keys correspond to what controls in-game, go to Window > Control
6. Using the available controls, select a level, option, mode, character, etc. and take note of these options
7. When you are finally at the first playable moment of the game, pause the game (in the integrator, not within the actual game) (``Command-P``), and save the state (``Command-S``). This moment can be hard to find, and you might have to go back through and restart the game (``Command-R``) to find and save that exact state.
8. Save the state — include the options you chose in the previous menus — e.g. ``SailorMoon.QueenBerylsCastle.Easy.Level1.state``

For Gym Retro integrations, a few notes about ROMs:

- We have preferred the USA version of ROMs, denoted by one of ``(USA)``, ``(USA, Europe)``, ``(Japan, USA)``, etc
- If the ROM has a ``.bin`` extension, rename it to have the correct extension for that system listed in :ref:`supported-roms`
- Use the Gym Retro Integration application and select the Integrate option from the File menu to begin working on integrating it

.. _supported-roms:

Supported ROM Types
--------------------------------------

ROM files contain the game itself. Each system has a unique file extension to denote which system a given ROM runs on:

- ``.md``: Sega Genesis (also known as Mega Drive)
- ``.sfc``: Super Nintendo Entertainment System (also known as Super Famicom)
- ``.nes``: Nintendo Entertainment System (also known as Famicom)
- ``.a26``: Atari 2600
- ``.gb``: Nintendo Game Boy
- ``.gba``: Nintendo Game Boy Advance
- ``.gbc``: Nintendo Game Boy Color
- ``.gg``: Sega Game Gear
- ``.pce``: NEC TurboGrafx-16 (also known as PC Engine)
- ``.sms``: Sega Master System

Sometimes ROMs from these systems use different extensions, e.g. ``.gen`` for Genesis, ``.bin`` for Atari, etc. Please rename the ROMs to use the aforementioned extensions in these cases.

Integrating a Game
--------------------------------------

To integrate a game you need to define a done condition and a reward function.  The done condition lets Gym Retro know when to end a game session, while the reward function provides a simple numeric goal for machine learning agents to maximize.

To define these, you find variables from the game's memory, such as the player's current score and lives remaining, and use those to create the done condition and reward function.  An example done condition is when the ``lives`` variable is equal to ``0``, an example reward function is the change in the ``score`` variable.

Note: if the game requires that you hit the ``Start`` button to play, for instance after dying, then you need to modify the scenario file to allow this as ``Start`` is disallowed by default.  See the ``actions`` key in `KidChameleon-Genesis <https://github.com/openai/retro/blob/master/retro/data/stable/KidChameleon-Genesis/scenario.json>`_ for an example of this.

Done Condition
~~~~~~~~~~~~~~~~~~~~~~

This is usually the easier of the two.  The best done condition to use is the Game Over or Continue screen after you run out of lives.  For some games this is when you have zero lives left, for some ``-1`` lives, for others, it can be pretty hard.

It's better to have a simple and reliable but slightly incorrect done condition (e.g. ending the game when you still have 1 life left because it's hard to detect the 0 life case) than to have a done condition that is unreliable, such as a ``gameover`` variable that detects when the gameover screen is present most of the time but also incorrectly fires when switching levels.

If you create a ``gameover`` variable, make sure to test it with a replay that plays multiple levels in a row to make sure it doesn't fire accidentally.

Reward Function
~~~~~~~~~~~~~~~~~~~~~~

Reinforcement learning agents try to maximize the reward function.  The ideal reward function would be that you get 1 point for beating the game.  There's no way to maximize that besides beating the game.

That reward is impractical though, because existing reinforcement learning algorithms are unable to make progress with a reward that is so hard to get.  Instead we can specify some easier to get reward that, if you maximize it, should result in beating the game.

If the game has a score, this is often a good choice.  In some games however, you can get as much score as you want by standing in one place and attacking the same enemy over and over as it respawns.  Because that is so different from beating the game, it's best to have an alternative reward, though these are often very game specific.

Be careful with non-score variables though, they can be tricky to get right, make sure to play multiple levels using the reward to see if it makes sense.

Provided you use the score, define a ``score`` variable and set the reward such that the reward the agent receives matches the score displayed on the screen, make sure to check that you're not off by a factor of 10 or 100 by comparing to the ``Cumulative`` value displayed in the ``Scenario Information`` pane of the UI.

Finding Variables
~~~~~~~~~~~~~~~~~~~~~~

It's best to keep a consistent pattern for the different types of variables you might add to a game's ``data.json`` file.  Here are some tips:

1. It's pretty common for multiple different variables to group themselves together. When narrowing down the search for a particular variable, look at nearby memory addresses if you suspect you have a similar but incorrect variable (for instance you found the high score variable but are looking for the score variable).
2. Score occasionally is stored in individual locations — e.g. if the score displayed is ``123400``, 1, 2, 3, 4, 0, 0 all will update separately.  If the score is broken into multiple variables, make sure you have penalties set for the individual digits (such as `BOB-Snes <https://github.com/openai/retro/blob/master/retro/data/stable/BOB-Snes/scenario.json>`_). A number of games will update the score value across multiple frames, in this case you will need a lua script to correct the reward, such as `1942-Nes <https://github.com/openai/retro/blob/master/retro/data/stable/1942-Nes/script.lua>`_.
3. Check for uncommon lengths of 3, 5, etc. Games don't always store score in nice neat lengths of multiples of 2, and making sure the variable is the appropriate length is key — if you go too short, then no progress over a certain score is tracked, if you go too long, then the score can suddenly jump between levels, etc.  If you can't decide between two possible lengths, the shorter length is the safer bet.
4. Score variable doesn't always include the 0s at the end of the game — while the screen might say ``2400``, the score variable might only store ``24``.  So you will need to multiply by 100 in this case.
5. It's very uncommon, but occasionally, scores can be transposed by a value of 1 — e.g., while the screen says ``123456``, the variable is ``012345``. Some of these scores start at -1 rather than 0. This can be fixed with lua.
6. It's very uncommon, but some games track health symbolically rather than with one set #. For example, the starting health bar could be represented by ``9999999``, which displays as a full health bar, but becomes ``99999`` after losing two health units.
7. In defining a game over variable, look for a binary value that switches between 0 and 1 – 0 when the game is in play, 1 when the game is over.  And make sure to test it by playing a few consecutive levels.

Once you've found a variable, making sure the address and type are correct is important for avoiding issues later.  One of the best ways to do this is to change the value in memory, then change it in the game and make sure it updates correctly.  

For instance if you have a variable called "score" and you want to see if it is ``>d2`` or ``>d4``, set the type to ``>d4`` and set the value to the maximum for ``>d2``, 9999, and then increase the score by playing the game.  If the score increases by 1, and the value in the memory viewer is 10000 and the value in the game is 10000, then ``>d4`` is correct.  If the value in the memory viewer or game is 0 or 9999, then it's likely that ``>d2`` is the correct type or that the address is wrong. You may also want to check if >d3 is the correct type by changing the score to 999999 and playing for a bit.

You can also check to see if the data type is correct by watching how it increments and decrements in the search window as you play the game. For example, if the value of the variable jumps from ``0`` to ``255``, it's likely that this is a signed value (represented by ``i``) -- unsigned values (represented by ``u``) are either positive or zero. 

When you search for a variable, different formats at the same location will appear next to each other in the search window. For example, at address ``16769105``, you might see ``>u2``, ``>i2``, as well as ``>d2`` return as search results. Play the game for a little bit, and you might notice that one of the search results increments/decrements in uneven or unusual ways in relation to the other search results at the same address.

eg: ``|u1`` at ``7e094d`` goes from ``144`` -> ``137``, ``|d1`` at ``7e094d`` (same address) decrements from ``90`` -> ``89``, it's probably ``|d1``

If you update the value of a variable but it doesn't have any effect on the game, it's likely that you've found a copy of the variable, not the correct address.  An example would be a lives variable, but setting it to some higher value and then dying in the game reveals that you didn't actually increase the number of lives.  It's often the case that you have to change the value in the game to get things to update (such as losing a life in the previous example).

Ideally you can find the original since it's more likely to be correct, so if you can, find a variable that when updated, updates the corresponding value in the game.  The most common source of this is a high score variable which will have the same value as the normal score variable, but updating it will have no effect on your score.

Common Errors
~~~~~~~~~~~~~~~~~~~~~~

* Wrong type for variable: if your score variable is actually ``>d2`` and you put ``>d4``, you may not notice until you get to some later level and the memory address next to the score is used for something, suddenly giving you a very large score.

* Incorrect done condition: it might be that if you run out of time or die in some unusual way that the done condition is not detected correctly.  Make sure to test unusual ways of ending the game, and make sure that your done condition doesn't fire upon completing a level (unless it's the final level of the game).  If you're able to hit continue after dying, make sure that the game ends before the agent can hit continue.

* Score is used as reward, but it's different from the score displayed in the game: this could happen if you forgot a factor of 10 in the reward, or if you're calculating the score based on some other variables (e.g. the upper and lower digits of the score, or some variable like ``number of enemies killed * 100``) and there is a bug.  If you play the game for awhile and the reward diverges slightly from the in-game score, it's possible that the score digits are not always updated at the same time.  In this case, you can use the change in maximum score as the reward, see `GuardianLegend-Nes <https://github.com/openai/retro/blob/master/retro/data/stable/GuardianLegend-Nes/script.lua>`_ for an example of this.

Using a Custom Integration from Python
--------------------------------------

Once you have created an integration, you can put it in a folder called ``custom_integrations``. In particular, your integration directory name should follow the following structure (depending on the rom type):

- ``.md``: ``*-Genesis`` (e.g. ``AddamsFamily-Genesis``)
- ``.sfc``: ``*-Snes`` (e.g. ``AddamsFamily-Snes``)
- ``.nes``: ``*-Nes`` (e.g. ``AddamsFamily-Nes``)
- ``.a26``: ``*-Atari2600`` (e.g. ``Adventure-Atari2600``)
- ``.gb``: ``*-GameBoy`` (e.g. ``AeroStar-GameBoy``)
- ``.gba``: ``*-GbAdvance`` (e.g. ``MarioKart-GbAdvance``)
- ``.gbc``: ``*-GbColor`` (e.g. ``PokemonPinball-GbColor``)
- ``.gg``: ``*-GameGear`` (e.g. ``MortalKombat-GameGear``)
- ``.pce``: ``*-PCEngine`` (e.g. ``SoldierBlade-PCEngine``)
- ``.sms``: ``*-Sms`` (e.g. ``AddamsFamily-Sms``)

Then you can tell ``retro`` about your custom integration using the ``add_custom_path`` function:

.. code-block:: python

	import retro
	import os

	SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


	def main():
		retro.data.Integrations.add_custom_path(
			os.path.join(SCRIPT_DIR, "custom_integrations")
		)
		print("FakeGame-Nes" in retro.data.list_games(inttype=retro.data.Integrations.ALL))
		env = retro.make("FakeGame-Nes", inttype=retro.data.Integrations.ALL)
		print(env)


	if __name__ == "__main__":
		main()

This lets you use your integration without having to add it to ``retro`` directly.

.. _appendix-types:

Appendix: Types
--------------------------------------

The types consist of three parts, in order:

- Endianness
- Format
- Bytes

Endianness refers to the order of the bytes in memory.
For example, take the hex string ``0x01020304``, which can be stored many ways:

 - Big endian: ``0x01 0x02 0x03 0x04``
 - Little endian: ``0x04 0x03 0x02 0x01``
 - Middle endian (big outside/little inside): ``0x02 0x01 0x04 0x03``
 - Middle endian (little outside/big inside): ``0x03 0x04 0x01 0x02``

The following sigils correspond to the endiannesses:

 - ``<``: Little
 - ``>``: Big
 - ``><``: Middle (big/little)
 - ``<>``: Middle (little/big)
 - ``=``: Native (little on most computers)
 - ``>=``: Middle (big/native)
 - ``<=``: Middle (little/native)
 - ``|``: Don't care (only useful for single-byte values)

*NB*: Middle endian is very rare, but some systems store 16-bit values in
native endian and 32-bit values as two 16-bit values in big endian order.
One such example is the emulator Genesis Plus GX. Thus, on a big endian
system the format appears to be ``=u4`` (aka ``>u4``) when it appears as ``>=u4``
on little endian systems. As such some data may require manual grooming.

Format refers to how in memory a value is stored.
For example, take the hex byte ``0x81``. It could mean three things in decimal:

 - Unsigned: 129
 - Signed: -127
 - Binary-coded decimal: 81
 - Low-nybble Binary-coded decimal: 1

*NB*: The nybbles ``0xA`` - ``0xF`` cannot occur in binary-coded decimal.

The following characters correspond to formats:

 - ``i``: Signed
 - ``u``: Unsigned
 - ``d``: Binary-coded Decimal
 - ``n``: Low-nybble Binary-coded Decimal

Finally, the last piece refers to how many bytes a value occupies in memory.
Ideally, this should be a power of two, e.g. 1, 2, 4, 8, etc., however non-power of two values are used by some games (e.g. the score in Super Mario Bros. is 6 bytes long), so non-power of two variables are supported.

*NB*: Native endian and middle endian don't work with non-power of two sizes or sizes less than 4 bytes. Currently only 4-byte middle endian is properly supported.

Some examples follow:

 - ``<u2``: Little endian two-byte unsigned value
   (i.e. ``0x0102`` -> ``0x02 0x01``)
 - ``<>u4``: Middle endian (little/big) four-byte unsigned value
   (i.e. ``0x01020304`` -> ``0x03 0x04 0x01 0x02``)
 - ``>d2``: Big endian two-byte binary-coded decimal value
   (i.e. ``1234`` -> ``0x12 0x34``)
 - ``|u1``: Single unsigned byte
 - ``<u3``: Non-power of two bytes
   (i.e. ``0x010203`` -> ``0x03 0x02 0x1``)
 - =n2: Native endian two-byte low-nybble binary-coded decimal value
   (i.e. ``12`` -> ``0x01 0x02`` on Intel and most ARM CPUs, ``0x02 0x01`` on PowerPC CPUs)

Some non-examples:

 - ``|i2``: Valid but not recommended: Two signed bytes, order undefined
 - ``<u1``: Valid but not recommended: One byte has no order
 - ``?u4``: Invalid: undefined endianness
 - ``>q2``: Invalid: undefined format
 - ``=i0``: Invalid: zero bytes
 - ``><u3``: Invalid: Non-power of two middle endian bytes
 - ``<=u2``: Invalid: Middle endian does not make sense for two byte values

.. _appendix-operations:

Appendix: Operations
--------------------------------------

Games can store information in memory in many various ways, and as such the specific information needed can vary in form too. The basic premise is that once a raw value is extracted from memory an operation may be defined to transform it to a useful form. Furthermore, we may want raw values in a given step or the deltas between two steps. Thus three properties are defined:

- ``measurement``: The method used for extracting the raw value. May be ``absolute`` for the current value and ``delta`` for the difference between the current and previous value. The default varies based on context.
- ``op``: The specific operation to apply to this value. Valid operations are defined below.
- ``reference``: The reference value for an operation, if needed.

The following operations are defined:

- ``nonzero``: Returns 0 if the value is 0, 1 otherwise.
- ``zero``: Returns 1 if the value is 0, 0 otherwise.
- ``positive``: Returns 1 if the value is positive, 0 otherwise.
- ``negative``: Returns 1 if the value is negative, 0 otherwise.
- ``sign``: Returns 1 if the value is positive, -1 if the value is negative, 0 otherwise.
- ``equal``: Returns 1 if the value is equal to the ``reference`` value, 0 otherwise.
- ``not-equal``: Returns 1 if the value is not equal to the ``reference`` value, 0 otherwise.
- ``less-than``: Returns 1 if the value is less than the ``reference`` value, 0 otherwise.
- ``greater-than``: Returns 1 if the value is greater than the ``reference`` value, 0 otherwise.
- ``less-or-equal``: Returns 1 if the value is less than or equal to the ``reference`` value, 0 otherwise.
- ``greater-or-equal``: Returns 1 if the value is greater than or equal to the ``reference`` value, 0 otherwise.
