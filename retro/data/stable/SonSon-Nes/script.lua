previous_score = 0

function correct_score ()
  if data.score > previous_score then
    local delta = data.score - previous_score
    previous_score = data.score
    return delta
  else
    return 0
  end
end
