previous_score = 0

function reward ()
  -- score is broken on last frame
  -- https://github.com/mgbellemare/Arcade-Learning-Environment/blob/master/src/games/supported/Atlantis.cpp#L62
  if data.lives == -1 then
    return 0
  end

  local score = data.score * 100 + data.scoreHi * 1000000
  local delta = score - previous_score
  previous_score = score
  return delta
end
