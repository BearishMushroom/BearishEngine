class Component extends ActorComponent {
  speed = 0;

  constructor() {
    base.constructor();
  }

  function Update() {
    base.Update();

    local total = 0;
    for(local i = 0; i < 30000; ++i) {
      total += i;
    }
  }
}
