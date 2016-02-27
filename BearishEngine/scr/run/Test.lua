Test = class()

function Test:new() end

function Test:Init()
	print("Starting...")
end

function Test:FixedUpdate()
	local p = vec2(20, 20)
	local l = vec2(40, 40)
	local r = p * l
end
