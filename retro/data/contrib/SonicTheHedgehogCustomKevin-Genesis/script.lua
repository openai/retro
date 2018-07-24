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

function clip(v, min, max)
    if v < min then
        return min
    elseif v > max then
        return max
    else
        return v
    end
end

prev_lives = 3

function contest_done()
    if data.lives < prev_lives then
        return true
    end
    prev_lives = data.lives

    if calc_progress(data) >= 1 then
        return true
    end

    return false
end

offset_x = nil
end_x = nil

function calc_progress(data)
    if offset_x == nil then
        offset_x = -data.x
        local key = string.format("zone=%d,act=%d", data.zone, data.act)
        end_x = level_max_x[key] - data.x
    end

    local cur_x = clip(data.x + offset_x, 0, end_x)
    return cur_x / end_x
end

prev_progress = 0
frame_count = 0
frame_limit = 18000

function contest_reward()
    frame_count = frame_count + 1
    local progress = calc_progress(data)
    local reward = (progress - prev_progress) * 9000
    prev_progress = progress

    -- bonus for beating level quickly
    if progress >= 1 then
        reward = reward + (1 - clip(frame_count/frame_limit, 0, 1)) * 1000
    end
    return reward
end
