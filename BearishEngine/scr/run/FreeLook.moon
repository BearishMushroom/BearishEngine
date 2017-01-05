export class FreeLookComponent
  new: (@sensitivity) =>
    @divisor = 100
    @id = "FreeLook"

  Serialize: =>
    {"sensitivity"}

  Init: =>
    Mouse.LockToCentre!

  FixedUpdate: =>
    if Mouse.IsButtonDown MouseButton.Left
      d = Mouse.GetDelta!
      @actor\GetTransform!\Rotate vec3(0, 1, 0), -d.x * (@sensitivity / @divisor)
      @actor\GetTransform!\Rotate @actor\GetTransform!\GetRotation!\Right!, -d.y * (@sensitivity / @divisor)
