max_score = 0

function max_score_reward ()
  local current_score = data.score
  if data.score > max_score then
    local delta = data.score - max_score
    max_score = data.score
    return delta
  else
    return 0
  end
end
-- Score briefly changes to 0 in between lives, thus doubling the reward.
-- This script takes that into account and compares the current score to the previous score and ignores jumps from 0 -> previous score.
