# Integrator's Guide

## How to integrate a game

1. Open Gym Retro Integration after setting up the UI.
2. Load a new game — `Command-Shift-O`
3. Select the ROM of the game you'd like to integrate in the menu.
4. Name the game.
5. The game will open. To see what keys correspond to what controls in-game, go to Window > Control.
6. Using the available controls, select a level, option, mode, character, etc. and take note of these options.
7. When you are finally at the first playable moment of the game, pause the game (in the integrator, not within the actual game) (`Command-P`), and save the state (`Command-S`). This moment can be hard to find, and you might have to go back through and restart the game (`Command-R`) to find and save that exact state.
8. Save the state — include the options you chose in the previous menus — e.g. `SailorMoon.QueenBerylsCastle.Easy.Level1.state`

## Integrating

To integrate a game you need to define a done condition and a reward function.  The done condition lets `gym-retro` know when to end a game session, while the reward function provides a simple numeric goal for machine learning agents to maximize.

To define these, you find variables from the game's memory, such as the player's current score and lives remaining, and use those to create the done condition and reward function.  An example done condition is when the `lives` variable is equal to `0`, an example reward function is the change in the `score` variable.

Note: if the game requires that you hit the `Start` button to play, for instance after dying, then you need to modify the scenario file to allow this as `Start` is disallowed by default.  See `data/KidChameleon-Genesis/scenario.json` for an example of this.

### Done Condition

This is usually the easier of the two.  The best done condition to use is the Game Over or Continue screen after you run out of lives.  For some games this is when you have zero lives left, for others `-1` lives, for others, it can be pretty hard.

It's better to have a simple and reliable but slightly incorrect done condition (e.g. ending the game when you still have 1 life left because it's hard to detect the 0 life case) than to have a done condition that is unreliable, such as a `gameover` variable that detects when the gameover screen is present.

If you create a `gameover` variable, make sure to test it with a replay that plays multiple levels in a row to make sure it doesn't fire accidentally.

### Reward Function

Reinforcement learning agents try to maximize the reward function.  The ideal reward function would be that you get 1 point for beating the game.  There's no way to maximize that besides beating the game.

That reward is impractical though, because existing reinforcement learning algorithms are unable to make progress with a reward that is so hard to get.  Instead we can specify some easier to get reward that, if you maximize it, should result in beating the game.

If the game has a score, this is often a good choice.  In some games however, you can get as much score as you want by standing in one place and attacking the same enemy over and over as it respawns.  Because that is so different from beating the game, it's best to have an alternative reward, though these are often very game specific.  Be careful with non-score variables though, they can be tricky to get right, make sure to play multiple levels using the reward to see if it makes sense.

Provided you use the score, define a `score` variable and set the reward such that the reward the agent receives matches the score displayed on the screen, make sure to check that you're not off by a factor of 10 or 100 by comparing to the Cumulative value displayed in the Scenario Information pane.

### What to look for in a game — is a game integratable?

1. Does the game have a score?
2. Is the score sufficiently dense (nice to have, not a need to have)
3. Does maximizing the `score` variable chosen accurately represent how a human might play the game?
4. Would a progress variable be more appropriate?
5. Does the game end or can you restart in-game ad infinitum?

#### Finding Variables

It's best to keep a consistent pattern for the different types of variables you might add to a game's `data` file.  Here are some tips:

1. It's pretty common for multiple different variables to group themselves together. When narrowing down the search for a particular variable, consider if it seems to be nearby other locations.
2. Score occasionally is stored in individual locations — e.g. if the score displayed is 123400, 1, 2, 3, 4, 0, 0 all will update separately.  If the score is broken into multiple variables, make sure you have penalties set for the individual digits.
3. Check for uncommon lengths of 3, 5, etc. Games don't always store score in nice neat lengths of multiples of 2, and making sure the variable is the appropriate length is key — if you go too short, then no progress over a certain score is tracked, if you go too long, then the score can suddenly jump between levels, etc.  If you can't decide between two possible lengths, the shorter length is the safer bet.
4. Score variable doesn't always include the 0s at the end of the game — while the screen might say 2400, the score variable might only store 24.
5. It's very uncommon, but occasionally, scores can be transposed by a value of 1 — e.g., while the screen says '123456', the variable is '012345'. Some of these scores start at -1 rather than 0. This can be fixed with lua.
7. It's very uncommon, but some games track health symbolically rather than with one set #. For example, the starting health bar could be represented by '9999999', which displays as a full health bar, but becomes '99999' after losing two health units.
8. In defining a game over variable, look for a binary value that switches between 0 and 1 – 0 when the game is in play, 1 when the game is over.

Once you've found a variable, making sure the address and type are correct is important for avoiding issues later.  One of the best ways to do this is to change the value in memory, then change it in the game and make sure it updates correctly.  For instance if you have a variable called "score" and you want to see if it is >d2 or >d4, set the type to >d4 and set the value to the maximum for >d2, 9999, and then increase the score by playing the game.  If the score increases by 1, and the value in the memory viewer is 10000 and the value in the game is 10000, then >d4 is correct.  If the value in the memory viewer or game is 0 or 9999, then it's likely that >d2 is the correct type or that the address is wrong. You may also want to check if >d3 is the correct type by changing the score to 999999 and playing for a bit.

You can also check to see if the data type is correct by watching how it increments and decrements in the search window as you play the game. For example, if the value of the variable jumps from `0` to `255`, it's likely that this is a signed value (represented by `i`) -- unsigned values (represented by `u`) are either positive or zero. When you search for a variable, different formats at the same location will appear next to each other in the search window. For example, at address `16769105`, you might see `>u2`, `>i2`, as well as `>d2` return as search results. Play the game for a little bit, and you might notice that one of the search results increments/decrements in uneven or unusual ways in relation to the other search results at the same address.

eg: |u1 at 7e094d goes from 144 -> 137, |d1 at 7e094d decrements from 90 -> 89, it's probably |d1

If you update the value of a variable but it doesn't have any effect on the game, it's likely that you've found a copy of the variable, not the correct address.  An example would be a lives variable, but setting it to some higher value and then dying in the game reveals that you didn't actually increase the number of lives.  It's often the case that you have to change the value in the game to get things to update (such as losing a life in the previous example).  Ideally you can find the original since it's more likely to be correct, so if you can, find a variable that when updated, updates the corresponding value in the game.  The most common source of this is a high score variable which will have the same value as the normal score variable, but updating it will have no effect on your score.

#### Common Errors

* Wrong type for variable: if your score variable is actually >d2 and you put >d4, you may not notice until you get to some later level and the memory address next to the score is used for something, suddenly giving you a very large score.

* Incorrect done condition: it might be that if you run out of time or die in some unusual way that the done condition is not detected correctly.  Make sure to test unusual ways of ending the game, and make sure that your done condition doesn't fire upon completing a level (unless it's the final level of the game).  If you're able to hit continue after dying, make sure that the game ends before the agent can hit continue.

* Score is used as reward, but it's different from the score displayed in the game: this could happen if you forgot a factor of 10 in the reward, or if you're calculating the score based on some other variables (e.g. the upper and lower digits of the score, or some variable like number of enemies killed * 100) and there is a bug.  If you play the game for awhile and the reward diverges slightly from the in-game score, it's possible that the score digits are not always updated at the same time.  In this case, you can use the change in maximum score as the reward, see `data/GuardianLegend-Nes/script.lua` for an example of this.
