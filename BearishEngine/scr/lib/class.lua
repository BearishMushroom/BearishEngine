class = {}
local classMetatable = {}

function classMetatable.__call(self, parent)
	local class = {}

	if type(parent) == 'table' then
		-- Copy the parent table.
		for i,v in pairs(parent) do
			class[i] = v
		end
		class.__parent = parent
	end

	class.__index = class

	local metatable = {}
	metatable.__call = function(class, ...)
		local instance = {}
		setmetatable(instance, class)

		if class.new then
			class.new(instance, ...)
		end

		if class.__parent then
			local base = class.__parent
			while base do
				base.new(instance, ...)
				base = base.__parent
			end
		end

		return instance
	end

	class.constructor = constructor
	class.is_a = function(self, class)
		local meta = getmetatable(self)
		while meta do
			if meta == class then return true end
			meta = meta.__parent
		end
		return false
	end

	setmetatable(class, metatable)
	return class
end

setmetatable(class, classMetatable)
