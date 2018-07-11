level_max_x = {
-- Green Hill Zone
    ["zone=0,act=0"] = 0x2560,
    ["zone=0,act=1"] = 0x1F60,
    ["zone=0,act=2"] = 0x292A,

-- Marble Zone
    ["zone=2,act=0"] = 0x1860,
    ["zone=2,act=1"] = 0x1860,
    ["zone=2,act=2"] = 0x1720,

-- Spring Yard Zone
    ["zone=4,act=0"] = 0x2360,
    ["zone=4,act=1"] = 0x2960,
    ["zone=4,act=2"] = 0x2B83,

-- Labyrinth Zone
    ["zone=1,act=0"] = 0x1A50,
    ["zone=1,act=1"] = 0x1150,
    ["zone=1,act=2"] = 0x1CC4,

-- Star Light Zone
    ["zone=3,act=0"] = 0x2060,
    ["zone=3,act=1"] = 0x2060,
    ["zone=3,act=2"] = 0x1F48,

-- Scrap Brain Zone
    ["zone=5,act=0"] = 0x2260,
    ["zone=5,act=1"] = 0x1EE0,
    -- ["zone=5,act=2"] = 000000, -- does not have a max x
}

function level_key()
    return string.format("zone=%d,act=%d", data.zone, data.act)
end

function clip(v, min, max)
    if v < min then
        return min
    elseif v > max then
        return max
    else
        return v
    end
end

data.prev_lives = 3

function contest_done()
    if data.lives < data.prev_lives then
        return true
    end
    data.prev_lives = data.lives

    if calc_progress(data) >= 1 then
        return true
    end

    return false
end

data.offset_x = nil
end_x = nil

function calc_progress(data)
    if data.offset_x == nil then
        data.offset_x = -data.x
        end_x = level_max_x[level_key()] - data.x
    end

    local cur_x = clip(data.x + data.offset_x, 0, end_x)
    return cur_x / end_x
end

data.prev_progress = 0
frame_limit = 18000

function contest_reward()
    local progress = calc_progress(data)
    local reward = (progress - data.prev_progress) * 9000
    data.prev_progress = progress

    -- bonus for beating level quickly
    if progress >= 1 then
        reward = reward + (1 - clip(scenario.frame / frame_limit, 0, 1)) * 1000
    end
    return reward
end

data.xpos_last_x = nil

function xpos_done()
    if data.lives < data.prev_lives then
        return true
    end
    data.prev_lives = data.lives

    if scenario.frame >= frame_limit then
        return true
    end

    return data.x > level_max_x[level_key()]
end

function xpos_rew()
    if data.xpos_last_x == nil then
        data.xpos_last_x = data.x
    end
    local result = data.x - data.xpos_last_x
    data.xpos_last_x = data.x
    return result
end
