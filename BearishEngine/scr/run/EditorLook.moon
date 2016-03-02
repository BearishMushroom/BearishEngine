export class EditorLookComponent
  new: =>

  FixedUpdate: =>
    if Keyboard.IsKeyDown Key.LeftAlt
      d = Mouse.GetDelta!

      if Mouse.IsButtonDown MouseButton.Left
        @actor\GetTransform!\Rotate vec3(0, 1, 0), -d.x * (0.002)
        @actor\GetTransform!\Rotate @actor\GetTransform!\GetRotation!\Right!, -d.y * (0.002)

      if Mouse.IsButtonDown MouseButton.Right
        tr = @actor\GetTransform!\GetTranslation!
        rot = @actor\GetTransform!\GetRotation!
        tr -= rot\Right! * vec3 0.005 * d.x
        tr -= rot\Down! * vec3 0.005 * d.y
        @actor\GetTransform!\SetTranslation tr

      if Mouse.IsButtonDown MouseButton.Middle
        tr = @actor\GetTransform!\GetTranslation!
        tr += @actor\GetTransform!\GetRotation!\Forward! * vec3 0.005 * d.y
        @actor\GetTransform!\SetTranslation tr
