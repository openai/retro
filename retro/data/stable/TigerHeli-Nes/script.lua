previous_score = 0
previous_stable_score = 0
stable_score = 0

-- score digits are sometimes updated high to low
-- make sure we have the same score for 2 frames in a row before updating the score
function correct_score()
  if data.score == previous_score then
    stable_score = data.score
  end
  previous_score = data.score

  local delta = stable_score - previous_stable_score
  previous_stable_score = stable_score
  return delta * 10
end
