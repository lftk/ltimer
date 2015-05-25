local ltimer = require("ltimer")
local timer = {id = 0, max_id = 4294967295, pool = {}}

function timer.cancel(id)
    if timer.pool[id] then
        timer.pool[id] = nil
        ltimer.delete(id)
    end
end

function timer.cancelall()
    timer.pool = {}
    ltimer.deleteall()
end

local function timeout(elapse_, func_, once_)
    if timer.id == timer.max_id then
        id = 1
    else
        id = timer.id + 1
    end
    while timer.pool[id] do
        id = id + 1
        if id == timer.id then
            error("timer id is used up")
        end
    end

    timer.id = id
    timer.pool[id] = {elapse = elapse_, func = func_, once = once_}
    ltimer.add(id, elapse_)
    return id
end

function timer.once(elapse, func)
    return timeout(elapse, func, true)
end

function timer.forever(elapse, func)
    return timeout(elapse, func, false)
end

function timer.expire()
    ltimer.expire(function(id)
        info = timer.pool[id]
        if info then
            if info.once then
                info.func()
                timer.pool[id] = nil
            else
                ltimer.add(id, info.elapse)
                info.func()
            end
        end
    end)
end

ltimer.init()

return timer
