Test = class()

function Test:new(pos)
	self.pos = pos
	self.secret = "Boo!"
end

function Test:Unspook()
  self.pos:Normalize()
	self.secret = "Yay!"
end

function Test:Print(x)
	print(self.secret, x, self.pos)
end
