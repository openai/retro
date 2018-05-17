function scores ()
  local score = data.score1
  if score == 192 then
    score = 100
  end
  local opponentScore = data.score2
  if opponentScore == 192 then
    opponentScore = 100
  end
  return score, opponentScore
end

oldrew = 0
function reward ()
  local score, opponentScore = scores()
  local rew = score - opponentScore
  newrew = rew - oldrew
  oldrew = rew
  return newrew
end

function done ()
  local score, opponentScore = scores()
  if score == 100 then
    return true
  end
  if opponentScore == 100 then
    return true
  end
  return data.minutes == 0 and data.seconds == 0
end
