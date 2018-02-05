prev_x = nil
prev_max_x = nil
prev_screen_x = nil
state_start, state_initialized, state_reset_wait = 'start', 'initialized', 'reset_wait'
state = state_start

function delta_x()
    local delta = data.x - prev_x
    local max_delta = data.screen_x_end - prev_max_x
    local screen_delta = data.screen_x - prev_screen_x
    if max_delta ~= 0 then
        delta = delta - screen_delta
    end
    prev_x = data.x
    prev_max_x = data.screen_x_end
    prev_screen_x = data.screen_x
    return delta
end

function x_and_level_reward()
    if state == state_start then
        if data.x ~= 0 then
            state = state_initialized
            prev_x = data.x
            prev_max_x = data.screen_x_end
            prev_screen_x = data.screen_x
        end
        return 0
    end

    delta = delta_x()
    if state == state_initialized then
        if data.level_end_bonus ~= 0 then
            state = state_reset_wait
            return 10000 + delta
        end
    elseif state == state_reset_wait then
        if data.x == 0 then
            state = state_start
            return 0
        end
    end
    return delta
end
