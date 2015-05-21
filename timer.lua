local ltimer = require "ltimer"

local tiemr_max_id = 4294967295
local timer_id = 0
local timer_pool = {}
local timer = {}

function timer.cancel(id)
	if timer_pool[id] then
		timer_pool[id] = nil
		ltimer.delete(id)
	end
end

function timer.cancelall()
	timer_pool = {}
	ltimer.deleteall()
end

local function timeout(elapse_, func_, once_)
	if timer_id == tiemr_max_id then
		id = 1
	else
		id = timer_id + 1
	end
	while timer_pool[id] do
		id = id + 1
		if id == timer_id then
			error("timer id is used up")
		end
	end

	timer_id = id
	timer_pool[id] = {elapse = elapse_, func = func_, once = once_}
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
		info = timer_pool[id]
		if info then
			if info["once"] then
				info["func"]()
				timer_pool[id] = nil
			else
				ltimer.add(id, info["elapse"])
				info["func"]()
			end
		end
	end)
end

ltimer.init()

return timer
