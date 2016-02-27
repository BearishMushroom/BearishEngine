export class Test2
  new: (@pos) =>
    @secret = "Boo!"

  Unspook: =>
    @pos\Normalize!
    @secret = "Yay!"

  Print: (x) =>
	   print @secret, x, @pos
