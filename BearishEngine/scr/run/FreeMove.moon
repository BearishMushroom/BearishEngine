export class FreeMoveComponent
  new: (@speed, @forward = Key.W, @backward = Key.S, @left = Key.A, @right = Key.D, @up = Key.E, @down = Key.Q) =>
    @id = "FreeMove"

  Serialize: =>
    {"speed", "forward", "backward", "left", "right", "up", "down"}

  FixedUpdate: =>
    tr = @actor\GetTransform!\GetTranslation!
    rot = @actor\GetTransform!\GetRotation!
    move = vec3 @speed, @speed, @speed

    if Keyboard.IsKeyDown @forward
      tr += rot\Forward! * move

    if Keyboard.IsKeyDown @backward
      tr += rot\Back! * move

    if Keyboard.IsKeyDown @left
      tr += rot\Left! * move

    if Keyboard.IsKeyDown @right
      tr += rot\Right! * move

    if Keyboard.IsKeyDown @up
      tr += rot\Up! * move

    if Keyboard.IsKeyDown @down
      tr += rot\Down! * move

    @actor\GetTransform!\SetTranslation tr
