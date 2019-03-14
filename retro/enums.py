from enum import Enum


class State(Enum):
    """
    Special values for setting the restart state of the environment.  You can
    also specify a string that is the name of the ``.state`` file
    """
    DEFAULT = -1  #: Start the game at the default savestate from ``metadata.json``
    NONE = 0  #: Start the game at the power on screen for the emulator


class Observations(Enum):
    """
    Different settings for the observation space of the environment
    """
    IMAGE = 0  #: Use RGB image observations
    RAM = 1  #: Use RAM observations where you can see the memory of the game instead of the screen


class Actions(Enum):
    """
    Different settings for the action space of the environment
    """
    ALL = 0  #: MultiBinary action space with no filtered actions
    FILTERED = 1  #: MultiBinary action space with invalid or not allowed actions filtered out
    DISCRETE = 2  #: Discrete action space for filtered actions
    MULTI_DISCRETE = 3  #: MultiDiscete action space for filtered actions