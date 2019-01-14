**Status:** Maintenance (expect bug fixes and minor updates)

# Gym Retro

Gym Retro is a wrapper for video game emulator cores using the Libretro API to turn them into Gym environments.
It includes support for several classic game consoles and a dataset of different games.

Supported platforms:

- Windows 7, 8, 10
- macOS 10.12 (Sierra), 10.13 (High Sierra), 10.14 (Mojave)
- Linux (manylinux1)

Supported Pythons:

- 3.5
- 3.6
- 3.7

Each game has files listing memory locations for in-game variables, reward functions based on those variables, episode end conditions, savestates at the beginning of levels and a file containing hashes of ROMs that work with these files.
Please note that ROMs are not included and you must obtain them yourself.
Most ROM hashes are sourced from their respective No-Intro SHA-1 sums.

Supported emulated systems:

- Atari
	- Atari2600 (via Stella)
- NEC
	- TurboGrafx-16/PC Engine (via Mednafen/Beetle PCE Fast)
- Nintendo
	- Game Boy/Game Boy Color (via gambatte)
	- Game Boy Advance (via mGBA)
	- Nintendo Entertainment System (via FCEUmm)
	- Super Nintendo Entertainment System (via Snes9x)
- Sega
	- GameGear (via Genesis Plus GX)
	- Genesis/Mega Drive (via Genesis Plus GX)
	- Master System (via Genesis Plus GX)

See [LICENSES.md](LICENSES.md) for information on the licenses of the individual cores.

# Installation

Gym Retro requires one of the supported versions of Python. Please make sure to install the appropriate distribution for your OS beforehand. Please note that due to compatibility issues with some of the cores, 32-bit operating systems are not supported.

```sh
pip3 install gym-retro
```

See the section [Install Retro from source](#install-retro-from-source) if you want to build gym-retro yourself (this is generally not necessary).

# Use With Gym

```python
import retro
env = retro.make(game='AirStriker-Genesis')
```

# Environments

What environments are there?  Note that this will display all defined environments, even ones for which ROMs are missing.

```python
import retro
retro.data.list_games()
```

What initial states are there?

```python
import retro
for game in retro.data.list_games():
    print(game, retro.data.list_states(game))
```

# Replay files

You can create and view replay files using the UI (Game > Play Movie...).  If you want to manage replay files in a script it looks like this:

## Record

```python
import retro

env = retro.make(game='AirStriker-Genesis', record='.')
env.reset()
while True:
    _obs, _rew, done, _info = env.step(env.action_space.sample())
    if done:
        break
```

## Playback

```python
import retro

movie = retro.Movie('AirStriker-Genesis-Level1-000000.bk2')
movie.step()

env = retro.make(game=movie.get_game(), state=None, use_restricted_actions=retro.Actions.ALL, players=movie.players)
env.initial_state = movie.get_state()
env.reset()

while movie.step():
    keys = []
    for p in range(movie.players):
        for i in range(env.num_buttons):
            keys.append(movie.get_key(i, p))
    _obs, _rew, _done, _info = env.step(keys)
```

## Render to Video

This requires ffmpeg to be installed and writes the output to the directory that the input file is located in.

```python
python -m retro.scripts.playback_movie AirStriker-Genesis-Level1-000000.bk2
```

# Integration User Interface

The integration UI helps you easily find variables and see what is going on with the reward function.  Binaries for this are not currently available, so you'll have to build this from source, see [Install Retro UI from source](#install-retro-ui-from-source).

# Development

## Install Retro from source

Building Gym Retro requires at least either gcc 5 or clang 3.4.

### Prerequisites

To build Gym Retro you must first install CMake.
You can do this either through your package manager, download from the [official site](https://cmake.org/download/) or `pip3 install cmake`.
If you're using the official installer on Windows, make sure to tell CMake to add itself to the system PATH.

### Mac prerequisites

Since LuaJIT does not work properly on macOS you must first install Lua 5.1 from homebrew:

```sh
brew install pkg-config lua@5.1
```

### Windows prerequisites

If you are not on Windows, please skip to the next section.
Otherwise, you will also need to download and install [Git](https://git-scm.com/downloads) and [MSYS2](http://www.msys2.org) x86_64.
When you install git, choose to use Git from the Windows Command Prompt.

After you have installed msys2 open an MSYS2 MinGW 64-bit prompt (under Start > MSYS2 64bit)  and run this command:

```sh
pacman -Sy make mingw-w64-x86_64-gcc
```

Once that's done, close the prompt and open a Git CMD prompt (under Start > Git) and run these commands.
If you installed MSYS2 into an alternate directory please use that instead of C:\msys64 in the command.

```sh
path %PATH%;C:\msys64\mingw64\bin;C:\msys64\usr\bin
set MSYSTEM=MINGW64
```

Then in the same prompt, without closing it first, continue with the steps in the next section.
If you close the prompt you will need to rerun the last commands before you can rebuild.

### Building

```sh
git clone --recursive https://github.com/openai/retro.git gym-retro
cd gym-retro
pip3 install -e .
```

### Updating submodules

When doing a `git pull` sometimes submodules will be updated. Usually this should be handled automatically, but in case of errors this can be quickly fixed by running the following steps before rebuilding:

```sh
git submodule deinit -f --all
rm -rf .git/modules
git submodule update --init
```

## Install Retro UI from source

First make sure you can install Retro from source, after that follow the instructions for your platform:

### macOS

Note that for Mojave (10.14) you may need to install `/Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg`

```sh
brew install pkg-config capnp lua@5.1 qt5
cmake . -DCMAKE_PREFIX_PATH=/usr/local/opt/qt -DBUILD_UI=ON -UPYLIB_DIRECTORY
make -j$(sysctl hw.ncpu | cut -d: -f2)
open "Gym Retro Integration.app"
```

### Linux

```sh
sudo apt-get install capnproto libcapnp-dev libqt5opengl5-dev qtbase5-dev
cmake . -DBUILD_UI=ON -UPYLIB_DIRECTORY
make -j$(grep -c ^processor /proc/cpuinfo)
./gym-retro-integration
```

### Windows

Building from source on Windows is currently difficult to configure. Docker containers for cross-compiling are available at [openai/travis-build](https://hub.docker.com/r/openai/travis-build/).

# Changelog

[See CHANGES.md](CHANGES.md)

# Example scripts

In the `examples` directory there are example scripts.

1. `random_agent.py`, loads up a given game and state file and picks random actions every step. It will print the current reward and will exit when the scenario is done. Note that it will throw an exception if no reward or scenario data is defined for that game. This script is useful to see if a scenario is properly set up and that the reward function isn't too generous.

# Add new ROMs

- We prefer the USA version of ROMs denoted by one of `(USA)`, `(USA, Europe)`, `(Japan, USA)`, etc.
- If the ROM has a `.bin` extension, rename it to have the [correct extension for that system](#rom-extensions).
- Use the Gym Retro Integration application and select the Integrate option from the File menu to begin working on integrating it.

# File formats

There are a handful of distinct file formats used.

<a name="rom-extensions"></a>
## ROMs

ROM files contain the game itself. Each system has a unique file extension to denote which system a given ROM runs on:

- `.md`: Sega Genesis (also known as Mega Drive)
- `.sfc`: Super Nintendo Entertainment System (also known as Super Famicom)
- `.nes`: Nintendo Entertainment System (also known as Famicom)
- `.a26`: Atari 2600
- `.gb`: Nintendo Game Boy
- `.gba`: Nintendo Game Boy Advance
- `.gbc`: Nintendo Game Boy Color
- `.gg`: Sega Game Gear
- `.pce`: NEC TurboGrafx-16 (also known as PC Engine)
- `.sms`: Sega Master System

Sometimes ROMs from these systems use different extensions, e.g. `.gen` for Genesis, `.bin` for Atari, etc. Please rename the ROMs to use the aforementioned extensions in these cases.
 
You can import your ROMs using [`retro.import`](https://github.com/openai/retro/blob/master/scripts/import.py). 

```sh
python3 -m retro.import /path/to/your/ROMs/directory/
```


The following non-commercial ROMs are included with Gym Retro for testing purposes:

- [the 128 sine-dot](http://www.pouet.net/prod.php?which=2762) by Anthrox
- [Sega Tween](https://pdroms.de/files/gamegear/sega-tween) by Ben Ryves
- [Happy 10!](http://www.pouet.net/prod.php?which=52716) by Blind IO
- [512-Colour Test Demo](https://pdroms.de/files/pcengine/512-colour-test-demo) by Chris Covell
- [Dekadrive](http://www.pouet.net/prod.php?which=67142) by Dekadence
- [Automaton](https://pdroms.de/files/atari2600/automaton-minigame-compo-2003) by Derek Ledbetter
- [Fire](http://privat.bahnhof.se/wb800787/gb/demo/64/) by dox
- [FamiCON intro](http://www.pouet.net/prod.php?which=53497) by dr88
- [Airstriker](https://pdroms.de/genesis/airstriker-v1-50-genesis-game) by Electrokinesis
- [Lost Marbles](https://pdroms.de/files/gameboyadvance/lost-marbles) by Vantage

## States

Emulation allows the entire state of a video game system to be stored to disk and restored. These files are specific to the emulator, but always end with `.state`. These are identical to the versions used in the standalone versions of the emulators but gzipped.

## Game information manifest (`data.json`)

Information about the inner workings of games are stored alongside the ROM in a file named `data.json`. This JSON file documents "ground truth" information about a game, including the locations and formats of variables in memory. These manifests are separated into sections, although only one section currently is defined:

### `info` section

The `info` section of the manifest lists game variables' memory addresses. Each entry in the `info` section consists of a key naming the memory address and the following values:

- `address`: The address into a RAM array of the first byte of the variable.
- `type`: A type descriptor for this variable. See the above addendum for the format of this value.

The following manifest shows an example of a game that has one variable, `score`, located at byte 128 that is 4 bytes wide in unsigned big endian format:

```json
{
	"info": {
		"score": {
			"address": 128,
			"type": ">u4"
		}
	}
}
```

### Addendum: Types

The types consist of three parts, in order:

- Endianness
- Format
- Bytes

Endianness refers to the order of the bytes in memory.
For example, take the hex string `0x01020304`, which can be stored many ways:

 - Big endian: `0x01 0x02 0x03 0x04`
 - Little endian: `0x04 0x03 0x02 0x01`
 - Middle endian (big outside/little inside): `0x02 0x01 0x04 0x03`
 - Middle endian (little outside/big inside): `0x03 0x04 0x01 0x02`

The following sigils correspond to the endiannesses:

 - `<`: Little
 - `>`: Big
 - `><`: Middle (big/little)
 - `<>`: Middle (little/big)
 - `=`: Native (little on most computers)
 - `>=`: Middle (big/native)
 - `<=`: Middle (little/native)
 - `|`: Don't care (only useful for single-byte values)

_NB_: Middle endian is very rare, but some systems store 16-bit values in
    native endian and 32-bit values as two 16-bit values in big endian order.
    One such example is the emulator Genesis Plus GX. Thus, on a big endian
    system the format appears to be `=u4` (aka `>u4`) when it appears as `>=u4`
    on little endian systems. As such some data may require manual grooming.

Format refers to how in memory a value is stored.
For example, take the hex byte `0x81`. It could mean three things in decimal:

 - Unsigned: 129
 - Signed: -127
 - Binary-coded decimal: 81
 - Low-nybble Binary-coded decimal: 1
NB: The nybbles `0xA` - `0xF` cannot occur in binary-coded decimal.

The following characters correspond to formats:

 - `i`: Signed
 - `u`: Unsigned
 - `d`: Binary-coded Decimal
 - `n`: Low-nybble Binary-coded Decimal

Finally, the last piece refers to how many bytes a value occupies in memory.
Ideally, this should be a power of two, e.g. 1, 2, 4, 8, etc., however non-power of two values are used by some games (e.g. the score in Super Mario Bros. is 6 bytes long), so non-power of two variables are supported.

_NB_: Native endian and middle endian don't work with non-power of two sizes or sizes less than 4 bytes. Currently only 4-byte middle endian is properly supported.

Some examples follow:

 - `<u2`: Little endian two-byte unsigned value
   (i.e. `0x0102` -> `0x02 0x01`)
 - `<>u4`: Middle endian (little/big) four-byte unsigned value
   (i.e. `0x01020304` -> `0x03 0x04 0x01 0x02`)
 - `>d2`: Big endian two-byte binary-coded decimal value
   (i.e. `1234` -> `0x12 0x34`)
 - `|u1`: Single unsigned byte
 - `<u3`: Non-power of two bytes
   (i.e. `0x010203` -> `0x03 0x02 0x1`)
 - =n2: Native endian two-byte low-nybble binary-coded decimal value
   (i.e. `12` -> `0x01 0x02` on Intel and most ARM CPUs, `0x02 0x01` on PowerPC CPUs)

Some non-examples:

 - `|i2`: Valid but not recommended: Two signed bytes, order undefined
 - `<u1`: Valid but not recommended: One byte has no order
 - `?u4`: Invalid: undefined endianness
 - `>q2`: Invalid: undefined format
 - `=i0`: Invalid: zero bytes
 - `><u3`: Invalid: Non-power of two middle endian bytes
 - `<=u2`: Invalid: Middle endian does not make sense for two byte values

## Scenario information (`scenario.json`)

Information pertaining to reward functions and done conditions can either be specified by manually overriding functions in `retro.RetroEnv` or can be done by writing a scenario file. Scenario files contain information that is used to compute the reward function and done condition from variables defined in the information manifest. Each variable specified in the scenario file is multiplied by a `reward` value if positive and a `penalty` value if negative and then summed up to create the reward for that step. Similarly, states of these variables can be checked to see if the game is over. By default the scenario file will be loaded from `scenario.json`, but alternative scenario files can be specified in the `retro.RetroEnv` constructor.

Scenario files are again JSON and specified with the following sections:

### `reward` section

The `reward` section used to calculate the reward function, and it split into the following subsections:

#### `variables` subsection

The `variables` subsection is used for defining how to calculate the reward function from the current state of memory. For each variable in the `variables` section, a value is calculated, multiplied by a coefficient, then added to the reward function for this step. How a value is extracted is specified by the `op`/`measurement`/`reference` values (see the addendum below on operations for the meanings of these). The default `measurement` is `delta`. There is no default `op`, and by default the value is passed through raw.

- `reward`: A coefficient multiplied by the value when the value is positive.
- `penalty`: A coefficient multiplied by the value when the value is negative.

_NB_: A negative `penalty` would imply addition to the reward function instead of subtraction as the value to be multiplied by the coefficient is negative.

#### `time` subsection

The `time` subsection is used for creating rewards based off of how many steps are taken. Two values can be specified:

- `reward`: A value to be added to the reward function every step.
- `penalty`: A value to be subtracted from the reward function every step.

### `done` section

The `done` section is used to calculate if the end of a game has been reached. At the top level the following property is available:

- `condition`: Specifies how the `done` conditions should be combined
  - `any`: Any of the conditions in the `done` section is fulfilled. This is the default.
  - `all`: All of the conditions in the `done` section are fulfilled.

Currently it has one subsection:

#### `variables` subsection

The `variables` subsection specifies how to calculate the done condition from the current state of memory. Each variable in the `variables` subsection is extracted per the `op`/`measurement`/`reference` values (see the addendum below on operations for the meanings of these). The default `measurement` is `absolute`. There is no default `op`, and by default the value is ignored.

### Addendum: Operations

Games can store information in memory in many various ways, and as such the specific information needed can vary in form too. The basic premise is that once a raw value is extracted from memory an operation may be defined to transform it to a useful form. Furthermore, we may want raw values in a given step or the deltas between two steps. Thus three properties are defined:

- `measurement`: The method used for extracting the raw value. May be `absolute` for the current value and `delta` for the difference between the current and previous value. The default varies based on context.
- `op`: The specific operation to apply to this value. Valid operations are defined below.
- `reference`: The reference value for an operation, if needed.

The following operations are defined:

- `nonzero`: Returns 0 if the value is 0, 1 otherwise.
- `zero`: Returns 1 if the value is 0, 0 otherwise.
- `positive`: Returns 1 if the value is positive, 0 otherwise.
- `negative`: Returns 1 if the value is negative, 0 otherwise.
- `sign`: Returns 1 if the value is positive, -1 if the value is negative, 0 otherwise.
- `equal`: Returns 1 if the value is equal to the `reference` value, 0 otherwise.
- `not-equal`: Returns 1 if the value is not equal to the `reference` value, 0 otherwise.
- `less-than`: Returns 1 if the value is less than the `reference` value, 0 otherwise.
- `greater-than`: Returns 1 if the value is greater than the `reference` value, 0 otherwise.
- `less-or-equal`: Returns 1 if the value is less than or equal to the `reference` value, 0 otherwise.
- `greater-or-equal`: Returns 1 if the value is greater than or equal to the `reference` value, 0 otherwise.
