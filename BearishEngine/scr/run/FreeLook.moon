export class FreeLookComponent
  new: (@sensitivity) =>
    @divisor = 100

  Init: =>
    Mouse.LockToCentre!

  FixedUpdate: =>
    d = Mouse.GetDelta!
    @actor\GetTransform!\Rotate vec3(0, 1, 0), -d.x * (@sensitivity / @divisor)
    @actor\GetTransform!\Rotate @actor\GetTransform!\GetRotation!\Right!, -d.y * (@sensitivity / @divisor)
