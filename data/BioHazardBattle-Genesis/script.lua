previous_score = 0

function correct_score ()
  local current_score = data.score
  if data.score > previous_score then
    local delta = data.score - previous_score
    previous_score = data.score
    return delta
  else
    return 0
  end
end

-- reward occasionally goes out of sync with the score variable, this puts them back in sync
