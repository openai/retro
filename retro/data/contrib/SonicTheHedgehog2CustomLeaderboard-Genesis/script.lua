level_max_x = {
-- Emerald Hill Zone
    ["zone=0,act=0"] = 0x2A40,
    ["zone=0,act=1"] = 0x29C0,

-- Chemical Plant Zone
    ["zone=13,act=0"] = 0x2840,
    ["zone=13,act=1"] = 0x2943,

-- Aquatic Ruin Zone
    ["zone=15,act=0"] = 0x298C,
    ["zone=15,act=1"] = 0x298D,

-- Casino Night Zone
    ["zone=12,act=0"] = 0x2840,
    ["zone=12,act=1"] = 0x2740,

-- Hill Top Zone
    ["zone=7,act=0"] = 0x2900,
    ["zone=7,act=1"] = 0x2E2E,

-- Mystic Cave Zone
    ["zone=11,act=0"] = 0x2450,
    ["zone=11,act=1"] = 0x21A0,

-- Oil Ocean Zone
    ["zone=10,act=0"] = 0x3040,
    ["zone=10,act=1"] = 0x2856,

-- Metropolis Zone
    ["zone=4,act=0"] = 0x2300,
    ["zone=4,act=1"] = 0x1F40,
    ["zone=5,act=0"] = 0x29AF,

-- Wing Fortress Zone
    ["zone=6,act=0"] = 0x29D9,
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
    if data.game_mode == 16 then
        -- bonus level
        return true
    end

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

    -- bonus for beating level
    if progress >= 1 then
        reward = reward + (1 - clip(frame_count/frame_limit, 0, 1)) * 1000
    end
    return reward
end
