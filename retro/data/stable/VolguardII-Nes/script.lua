function digit(value)
  return value - 1
end

oldvalue = 0

function reward()
  value = digit(data.score0)
  value = value + digit(data.score1) * 10
  value = value + digit(data.score2) * 100
  value = value + digit(data.score3) * 1000
  value = value + digit(data.score4) * 10000
  value = value + digit(data.score5) * 100000
  value = value + digit(data.score6) * 1000000
  value = value + digit(data.score7) * 10000000
  newvalue = value - oldvalue
  oldvalue = value
  return newvalue
end
