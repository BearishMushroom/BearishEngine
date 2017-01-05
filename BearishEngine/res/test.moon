class Test
  new: (x = 0, y = 0) =>
    @x = x
    @y = y

  add: (x, y = x) =>
    @x += x
    @y += y

  printMe: =>
    print @x, @y

var = Test(20)
var\printMe! -- Should print 20, 0
var\add 10
var\printMe! -- 30, 10

-- MoonScript ^
-- Same code. Does supporting both make sense? I think so.
-- Luva       v

local class Test(x => 0, y => 0) @test {
  public x, y = x, y

  public static:
    function add(x => 0, y => x) {
      self.x += x
      self.y += y
    }

    function printMe() {
      print(self.x, self.y)
    }
}

local var = Test(20)
var:printMe()
var:add(10)
var:printMe()

-- My own proposal

class Test
  private x
  public y

  function new(_x = 0, _y = 0)
    x = _x
    y = _y
  end

  function add(xa, ya = xa)
    x += xa
    y += ya
  end

  function printMe()
    print(x, y)
  end
end

var l = Test(20)
l.printMe()
l.add(10)
l.printMe()

-- Generates

function Test(_x, _y)
  _x = _x or 0
  _y = _y or 0

  local self = {
    y = 0,
  }
  local x = 0

  x = _x
  y = _y

  function self.add(xa, ya)
    ya = ya or xa
    x = x + xa
    self.y = self.y + ya
  end

  function self.printMe()
    print(x, self.y)
  end
end

local l = Test(20)
l.printMe()
l.add(10)
l.printMe()
