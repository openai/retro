function done ()
  if data.matches < 6 and data.opponentMatches < 6 then
    return false
  end
  if data.matches - data.opponentMatches >= 2 then
    return true
  end
  if data.opponentMatches - data.matches >= 2 then
    return true
  end
  return false
end
