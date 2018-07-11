level_max_x = {
-- Angel Island Zone
    ["zone=0,act=0"] = 0x3080 + 0x1049,
    ["zone=0,act=1"] = 0x4870,

-- Hydrocity Zone
    ["zone=1,act=0"] = 0x3630,
    ["zone=1,act=1"] = 0x3F80,

-- Marble Garden Zone
    ["zone=2,act=0"] = 0x2D16,
    ["zone=2,act=1"] = 0x3B62,

-- Carnival Night Zone
    ["zone=3,act=0"] = 0x3163,
    ["zone=3,act=1"] = 0x4786,

-- Icecap Zone
    ["zone=5,act=0"] = 0x68A7, -- changes act before boss
    ["zone=5,act=1"] = 0x4342,

-- Launch Base Zone
    ["zone=6,act=0"] = 0x3D4A,
    ["zone=6,act=1"] = 0x39A3,

-- Mushroom Hill Zone
    ["zone=7,act=0"] = 0x420B,
    ["zone=7,act=1"] = 0x3D41,

-- Flying Battery Zone
    ["zone=4,act=0"] = 0x2DDD,
    ["zone=4,act=1"] = 0x33AD,

-- Sandopolis Zone
    ["zone=8,act=0"] = 0x40FB,
    ["zone=8,act=1"] = 0x5109,

-- Lava Reef Zone
    ["zone=9,act=0"] = 0x2BE4,
    ["zone=9,act=1"] = 0x38B0,

-- Hidden Palace Zone
    ["zone=22,act=1"] = 0x0FC8,

-- Death Egg Zone
    ["zone=11,act=0"] = 0x35EE,
    ["zone=11,act=1"] = 0x359F,
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
    if data.special_stage ~= 0 then
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
data.prev_screen_x = nil
data.prev_act = nil

function calc_progress(data)
    if data.offset_x == nil then
        data.offset_x = -data.x
        data.prev_act = data.act
        data.prev_screen_x = data.screen_x
        end_x = level_max_x[level_key()] - data.x
    end

    -- when the act changes in the middle of a level, adjust offset_x to compensate
    if data.act ~= data.prev_act then
        data.offset_x = data.offset_x - (data.screen_x - data.prev_screen_x)
    end

    local cur_x = clip(data.x + data.offset_x, 0, end_x)

    data.prev_act = data.act
    data.prev_screen_x = data.screen_x

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
    -- Some levels change act mid-way through.
    -- This should not be allowed by the max x, but we
    -- still have a fallback.
    level_max_x["zone=0,act=0"] = 0x2f20
    if data.prev_act == nil then
        data.prev_act = data.act
    end
    if data.act ~= data.prev_act then
        return true
    end

    if data.special_stage ~= 0 then
        return true
    end

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
