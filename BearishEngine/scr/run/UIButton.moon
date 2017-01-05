export class UIButton
  pointInVec4 = (rect, point) ->
    point.x >= rect.x and point.x <= rect.z and
      point.y <= rect.w and point.y >= rect.y

  new: (@texture, @action = nil) =>
    @id = "UIButton"

  Init: =>
    @mesh = Mesh.CreateQuad vec4 0, 0, 1, 1

  FixedUpdate: =>
    offset = if @actor\HasParent! then @actor\GetParent!\GetTranslation! else vec3 0, 0, 0
    model = Transform @actor\GetTransform!

    position = model\GetTranslation!.xy + vec2 offset.x, offset.y
    size = model\GetScale!.xy

    rect = vec4 position.x - size.x / 2, position.y - size.y / 2, position.x + size.x / 2, position.y + size.y / 2

    if Mouse.IsButtonReleased(MouseButton.Left) and pointInVec4 rect, Mouse.GetPosition!
      @action @ if @action

  Draw2D: (renderer, shader, camera) =>
    offset = if @actor\HasParent! then @actor\GetParent!\GetTransform!\GetTranslation! else vec3 0, 0, 0
    model = Transform @actor\GetTransform!
    model\SetTranslation model\GetTranslation! + vec3 offset.x, offset.y, -1

    @mesh\Submit nil, model\GetTransformation!, camera

    @texture\Bind 0, 0
    @mesh\Flush!
    @texture\Unbind 0
