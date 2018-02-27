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
    if data.special_stage ~= 0 then
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
prev_screen_x = nil
prev_act = nil

function calc_progress(data)
    if offset_x == nil then
        offset_x = -data.x
        prev_act = data.act
        prev_screen_x = data.screen_x
        local key = string.format("zone=%d,act=%d", data.zone, data.act)
        end_x = level_max_x[key] - data.x
    end

    -- when the act changes in the middle of a level, adjust offset_x to compensate
    if data.act ~= prev_act then
        offset_x = offset_x - (data.screen_x - prev_screen_x)
    end

    local cur_x = clip(data.x + offset_x, 0, end_x)

    prev_act = data.act
    prev_screen_x = data.screen_x

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


