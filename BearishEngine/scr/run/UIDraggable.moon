UIDragPosition =
  Top: 0
  Bottom: 1
  Left: 2
  Right: 3
  Centre: 4

export UIDragPosition

export class UIDraggable
  pointInVec4 = (rect, point) ->
    point.x >= rect.x and point.x <= rect.z and
      point.y <= rect.w and point.y >= rect.y

  new: (@pos = 0, @margin = 50) =>
    @isDragging = false
    @rect = vec4 0, 0, 0, 0
    @dragPos = vec2 0, 0

  FixedUpdate: =>
    offset = if @actor\HasParent! then @actor\GetParent!\GetTransform!\GetTranslation! else vec3 0, 0, 0
    model = @actor\GetTransform!
    model\SetTranslation model\GetTranslation! + vec3 offset.x, offset.y, 0

    position = model\GetTranslation!.xy
    size = model\GetScale!.xy

    @rect = switch @pos
      when UIDragPosition.Top
        vec4 position.x - size.x / 2, position.y - size.y / 2, position.x + size.x / 2, position.y - size.y / 2 + @margin
      when UIDragPosition.Bottom
        vec4 position.x - size.x / 2, position.y + size.y / 2 - @margin, position.x + size.x / 2, position.y + size.y / 2
      when UIDragPosition.Left
        vec4 position.x - size.x / 2, position.y - size.y / 2, position.x - size.x / 2 + @margin, position.y + size.y / 2
      when UIDragPosition.Right
        vec4 position.x + size.x / 2 - @margin, position.y - size.y / 2, position.x + size.x / 2 - @margin, position.y - size.y / 2
      when UIDragPosition.Centre
        vec4 position.x - size.x / 2 + @margin, position.y - size.y / 2 + @margin, position.x + size.x / 2 - @margin, position.y + size.y / 2 - @margin
      else
        vec4 0, 0, 0, 0

    if Mouse.IsButtonPressed MouseButton.Left
      @isDragging = pointInVec4 @rect, Mouse.GetPosition!
      @dragPos = position - Mouse.GetPosition!
      print "Mouse", @rect, @pos, UIDragPosition

    if Mouse.IsButtonDown(MouseButton.Left) and @isDragging
      newpos = Mouse.GetPosition! + @dragPos
      @actor\GetTransform!\SetTranslation vec3(newpos.x, newpos.y, 0) - offset
