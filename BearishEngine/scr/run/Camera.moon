export class CameraComponent
  new: =>
    @id = "Camera"

  Init: =>
    @camera = Camera mat4!\CreateProjection(70 * 3 / 180, 16 / 9, 0.1, 100),
      Transform(vec3(0, 0, 0), vec3(1, 1, 1), quat(0, 0, 0, 1))

  Update: =>
    @camera\SetTransform @actor\GetTransform!

  PreDraw: (renderer, camera) =>
    renderer\SetCamera @camera
