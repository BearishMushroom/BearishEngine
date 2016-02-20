class CameraComponent extends ActorComponent {
  camera = null;

  constructor(projection) {
    base.constructor();
    this.camera = Camera(projection, Transform(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), quat(0.0, 0.0, 0.0, 1.0)));
  }

  function Update() {
    base.Update();
    this.camera.SetTransform(GetParent().GetTransform());
  }

  function PreDraw(renderingEngine, camera) {
    base.PreDraw(renderingEngine, camera);
    renderingEngine.SetCamera(this.camera);
  }
}
