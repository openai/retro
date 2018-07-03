previous_score = 0

function correct_score ()
  local score = data.score * 100
  if score > previous_score then
    local delta = score - previous_score
    previous_score = score
    return delta
  else
    return 0
  end
end
