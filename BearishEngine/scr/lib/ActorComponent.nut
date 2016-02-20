// Kind of a proxy class that makes sure that no errors
// are thrown if the user doesn't implement all the
// methods in the IActorComponent C++ class.

class ActorComponent extends IActorComponent {
  constructor() {
    base.constructor();
  }

  function Init() {
    base.Init();
  }

  function Update() {
    base.Update();
  }

  function FixedUpdate() {
    base.FixedUpdate();
  }

  function PreDraw(renderingEngine, camera) {
    base.PreDraw(renderingEngine, camera);
  }

  function Draw(renderingEngine, shader, camera) {
    base.Draw(renderingEngine, shader, camera);
  }

  function PostDraw(renderingEngine, camera) {
    base.PostDraw(renderingEngine, camera);
  }
}
