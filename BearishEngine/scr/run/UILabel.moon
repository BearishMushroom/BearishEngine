export class UILabel
  new: (@font, @text, @scale) =>
    @mesh = @font\GenerateMesh @text, @scale

  Draw2D: (renderer, shader, camera) =>
    offset = if @actor\HasParent! then @actor\GetParent!\GetTranslation! else vec3 0, 0, 0
    model = Transform @actor\GetTransform!
    model\SetTranslation model\GetTranslation! + vec3 offset.x, offset.y, -1

    @mesh\Submit model\GetTransformation!, camera\GetViewMatrix! * model\GetTransformation!

    @font\GetTexture!\Bind 0, 0
    @mesh\Flush!
    @font\GetTexture!\Unbind 0

  SetText: (text, scale) =>
    @text = text
    @scale = scale
    @mesh = @font\GenerateMesh @text, @scale
