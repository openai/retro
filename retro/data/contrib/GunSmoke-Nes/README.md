Gun.Smoke (USA).nes
===================

Included example scenario
-------------------------

Done criteria, one of:

- Lives goes to 0: Fairly obvious.
- Map tile wraps around: Since Gun.Smoke loops until the player finds (or buys)
  the wanted poster and thereby finds the level boss, end the game if we enter
  another loop of the course. All courses have a hidden wanted poster that can
  be found before the actual loop.
- Shop select equals less than 5: This value is initialized to 14, meaning that
  we can detect when an actual selection (0-4) is active, and end the level
  prematurely. This will help us avoid any AI entering the shop, as we don't
  include select (shop exit) in the possible inputs in this example.
- Wanted flag changes in a negative direction: Just when the boss has been
  killed, the wanted flag goes to 0, meaning that we have won. This is also
  indicated by a map tile wraparound (see above) when the player enters the new
  map, but that happens much much later.

Reward function:

- Boots speed: Encourage the AI to pick up boot speedups, and to not pick up
  skulls. Will not affect a run, as the game is an autoscroller.
- Rifle range: Encourage the AI to pick up rifle range extenders, see above.
- Lives: Encourage the AI to pick up extra life items
- Money: Divided into 6 memory locations that store the font locations used to
  display the money amount. Simply masked and multiplied with its values.
- Boss health: Encourage the AI to find the boss, and kill it quickly.
- Wanted flag: Encourage the AI to continue exploring paths where a wanted
  poster is found.
