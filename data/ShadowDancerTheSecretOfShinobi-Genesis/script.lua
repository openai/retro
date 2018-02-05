function scores ()
  local score = data.score
  local x_position = data.scroll
  if x_position == 50 then
    score = 0
  end
  return score
end

oldrew = 0
function reward ()
  local score = scores()
  local rew = score 
  newrew = rew - oldrew
  oldrew = rew
  return newrew
end
