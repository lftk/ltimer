local timer = require "timer"

id1 = timer.once(200, function()
    print("once " .. id1)
end)

id2 = timer.forever(300, function()
    print("forever " .. id2)
end)

id3 = timer.once(700, function()
    print("once " .. id3)
    timer.cancel(id2)
    id31 = timer.once(800, function()
        print(id3 .. " once " .. id31)
    end)
end)

id4 = timer.forever(1000, function()
    print("forever " .. id4)
end)

function ff()
    print(timer.once(1000, ff))
end

ff()

while true do
    timer.expire()
end
