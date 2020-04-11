# Changelog

## 0.8.0

* add python 3.8 support
* drop python 3.5 due to build issues on windows

## 0.7.1

* fix discrete and multi-discrete action space filtering
* fix random printfs when making environments
* data fixes for AeroStar-GameBoy, ChaseHQII-Genesis, Geimos-Nes, MagicalTaruruutoKun-Genesis,  KanshakudamaNageKantarouNoToukaidouGojuusanTsugi-Nes, TigerHeli-Nes (may change reward for these games, mostly these are bug fixes)
# python 3.5 compatibility fix (thanks @kieran-lemayai!)
* fix for new pyglet minor version that breaks backward compatibility (thanks @fsimond!)
* json parsing fix (thanks @eaplatanios!)
* minor memory leak fix (thanks @eaplatanios!)

## 0.7.0

* move some buggy games from the `stable` integrations folder to `experimental`
* minor bug fixes including fixes to a few game scenarios
* more docs
* add ability to use arbitrary additional integration directories
* integration UI searches for current Python's Gym Retro data directory
* import script can now accept files in addition to directories
* you can now use RAM observations by sending `obs_type=retro.Observations.RAM` to `retro.make`
* update Atari 2600 emulator

## 0.6.0

* add cores for GB/C, GBA, GG, NES, SMS, SNES, TurboGrafx
* add integration UI and searching
* add basic scenario access to Lua
* improve testing tooling
* multi-agent support
* cleaned up API:
  * everything involving data, e.g. game and state listing, file lookup and data path handling, has been moved into retro.data
  * importing retro.data.experiment or retro.data.contrib includes additional games and data that may not be as well-tested
  * retro.ACTIONS_* and retro.STATE_* have been replaced with retro.Actions.* and retro.State.* enums
  * retro.data.GameData objects no longer need an associated RetroEmulator object, though some functionality will not work
* add screen cropping
* added RetroEnv.get_action_meaning to describe the correlation between actions and buttons
* fixed d-pad action filtering so e.g. UP+DOWN+LEFT reduces to LEFT instead of NOOP
* add parallelism, lossless videos, info dict, disabling audio and numpy action dumping to playback_movies
* update LuaJIT to 2.1.0-beta3

## 0.5.6

* fix generating corrupted bk2s
* reliabilty fixes and minor enhancements to playback_movies

## 0.5.5

* allow Atari height to be different per game
* update pybind11 dependency
* add parallelism, lossless videos, info dict and numpy action dumping to playback_movies
* fix crashes with TensorFlow

## 0.5.4

* improved Windows support
* refreshed Python memory access
* build manylinux1-compatible wheels
* minor documentation fixes
* minor fixes to playback_movie script
* update Atari 2600 emulator

## 0.5.3

* fix Lua on Windows
* add Windows support to import.sega_classics
* only use system libzip if compatible

## 0.5.2

* initial public release
