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
    if data.game_mode == 16 then
        -- bonus level
        return true
    end

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

    -- bonus for beating level
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

    if data.game_mode == 16 then
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
