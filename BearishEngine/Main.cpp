#include "src/Bearish.h"
#include <vector>

using namespace Bearish;

using namespace Components;
using namespace Graphics;
using namespace Core;
using namespace Math;
using namespace Util;

	/*------------- TERRIBLE CHANGELOG THINGY -------------*\
	 0.011a
		- Simple shader loading and uniform support.
		- Logging.

	 0.013a
		- Wrappers for VBO and IBO.
		- Simple Renderer class.
		- Simple rendering loop.

	 0.0131a
		- Fixed allocator issues.

	0.015a
		- Added a working transform class.
		- Cleaned up math headers.

	0.016a
		- Keyboard input, ho!
		- Mouse input, too!

	0.017a
		- Textures.

	0.018a
		- Directional lighting.

	0.019a
		- Much more advanced shader pre-processing.
		- Now finds and parses structs as datatypes and recursive uniforms.
		- Also added functions to automatically set all uniforms for lights.

	0.020a
		- Point light support.

	0.022a
		- Spot lights implemented.
		- Added simple model loading.
		- Added a very crude resource-cache to limit 
		  reloading of same models.

	0.023a
		- Added Actor class.
		- Added IActorComponent interface.
		- Created MeshRenderer component.
		- Restructured sample scene to use new systems.
		- Basic FBO support.

	0.024a
		- Added a trigger system to Actors and Components.
		- Made better use of references for performance in loops.

	0.025a
		- Shadowmapping for directional lights.
		- RenderingEngine class to contain rendering code.

	0.026a
		- Skybox component.

	0.027a
		- Refactored things into components.
		- Made component names more consistant.

	0.043a
		- Added a general scripting engine (Squirrel) and
		  added support for creating components entirely
		  in squirrel.
		- Added debug views on F1, F2, and F3.

	0.044a
		- Added VAO encapsulation class.
		- Moved all mesh structure into VAOs.
		- Global rendering state no longer changes vertex
		  attributes as this can now be stored in VAOs.

	0.046a
		- Implmented instanced rendering with a primitive
		  rendering queue.

	0.160a
		- Holy shit I did everything.
	\*-----------------------------------------------------*/

	/*------------- MASSIVE TODO LIST OF DOOM --------------*\
	O Wrapper class for IBO's.                          (100%)
	O Intregrate lights into object system.             (100%)
	O Transform class.                                  (100%)
	O Expose math to Squirrel.                          (100%)
	O Seperate VBOs for different attributes.           (100%)
	O Move VBO state into VAO objects.                  (100%)
	O Look into deferred rendering.                     (100%)
	O Move logging away from preprocessor nightmare.    (100%)
	O Get attributes rendered into seperate textures.   (100%)
	- Texture class.                                    ( 80%)
	- Framebuffer objects.                              ( 80%)
	- Input. (KB, mouse, gamepad)                       ( 75%)
	- Simple mesh structure.                            ( 70%)
	- Automate shader loading.                          ( 70%)
	- More generic lighting structure.                  ( 70%)
	- Apply simple lighting.                            ( 70%)
	- Simple shading pipeline.                          ( 70%)
	- Move stuff into actual generic renderer.          ( 70%)
	- Simple shadowmapping.                             ( 60%)
	- Renderer class.                                   ( 60%)
	- Material rendering.                               ( 50%)
	- Particles. Yay.                                   ( 50%)
	- Font rendering.                                   ( 50%)
	- Start planning "scene" structure.                 ( 30%)
	- Expose core to Squirrel.                          ( 30%)
	- Model loading (AssIMP)                            ( 20%)
	- GUI rendering.                                    ( 20%)
	- Expose graphics to Squirrel.                      ( 15%)
	- PCF shadows.                                      (  0%)
	- Some sort of settings file loaded by the engine.  (  0%)
	- Upgrade build system to MSVC15.					(  0%)
	\*------------------------------------------------------*/

	/*-------------------------BUGS-------------------------*\
	- Mesh(Model) constructor returns an undefined mesh.
	- IAllocatable doesn't work for polymorphic objects, 
	  maybe re-think how it's done?
	- Function overloads with same num of args is unsupported
		in Sqrat, but can be worked around with SquirrelFunc,
		use this to supply better operator support.
	\*------------------------------------------------------*/


// Chaiscript has proven to be waaaay too slow for our purpose.
// The next langauge in line is Squirrel, most known for being used in Source.
// The SQPlus library will be used to automate the binding of classes to Squirrel.
// Edit: SQPlus has now been replaced by SQRat.
// Build times are also a lot better than with chai, as chai rebuilt the entire
//	scripting language each time you changed binding code.

// Rethinking scripting again.
// Perhaps we want a Lua-based model using meta tables as "classes" and components.

#define UPDATE_TIME 1.0 / 60.0


Actor* object1 = new Actor(Transform(vec3(0, 0, 0), vec3(0.1f)));
Actor* object2 = new Actor(Transform(vec3(1.5f, 0, 0), vec3(0.1f)));
Actor* object3 = new Actor(Transform(vec3(-1.5f, 0, 0), vec3(0.1f)));

Actor* fpsCounter = new Actor(Transform(vec3(10, 40, -2)));

std::vector<Actor*> actors;
RenderingEngine renderer;

Callback<i32, i32> testCB;
Action<std::vector<i32>> testAC;

std::unordered_map<string, string> Assets::_values;

#include <lua.hpp>

// Temporary update solution.
void Update() {
	for (i32 i = 0; i < (i32)actors.size(); i++) {
		if (actors.at(i)->IsDead()) {
			Actor* todel = actors.at(i);
			actors.insert(actors.begin() + i, actors.at(actors.size() - 1));
			actors.resize(actors.size() - 1);
			delete todel;
		}
	}

	for (auto& a : actors) {
		a->FixedUpdate();
	}

	//object1->GetTransform().Rotate(vec3(0, 1, 0), -100.f);
	//object2->GetTransform().Rotate(vec3(1, 0, 0), -100.f);
	//object3->GetTransform().Rotate(vec3(0, 0, 1), -100.f);

	if (Keyboard::IsKeyPressed(Key::F1)) {
		Renderer::SetFillMode(FillMode::Normal);
	} 
	
	if (Keyboard::IsKeyPressed(Key::F2)) {
		Renderer::SetFillMode(FillMode::Wireframe);
	}

	if (Keyboard::IsKeyPressed(Key::F3)) {
		Renderer::SetFillMode(FillMode::Points);
	}

	if (Keyboard::IsKeyPressed(Key::F4)) {
		renderer.SetDebugDeffered(false);
	}

	if (Keyboard::IsKeyPressed(Key::F5)) {
		renderer.SetDebugDeffered(true);
	}

	if (Keyboard::IsKeyPressed(Key::F6)) {
		renderer.SetShadowQuality(ShadowQuality::Off);
	}

	if (Keyboard::IsKeyPressed(Key::F7)) {
		renderer.SetShadowQuality(ShadowQuality::Terrible);
	}

	if (Keyboard::IsKeyPressed(Key::F8)) {
		renderer.SetShadowQuality(ShadowQuality::Low);
	}
	
	if (Keyboard::IsKeyPressed(Key::F9)) {
		renderer.SetShadowQuality(ShadowQuality::Medium);
	}
	
	if (Keyboard::IsKeyPressed(Key::F10)) {
		renderer.SetShadowQuality(ShadowQuality::High);
	}

	if (Keyboard::IsKeyPressed(Key::F11)) {
		renderer.SetShadowQuality(ShadowQuality::Ultra);
	}

	if (Keyboard::IsKeyPressed(Key::L)) {
		Actor* panelTest = new Actor(Transform(vec3(400, 400, -1), vec3(300, 230, 1)));
		panelTest->AddComponent(new UIPanel(new Texture(vec4(1, 0, 1, 1))));
		panelTest->AddComponent<UIDraggable>();

		Actor* buttonTest = new Actor(Transform(vec3(130, -85, -2), vec3(30, 30, 1)));
		buttonTest->AddComponent(new UIButton(new Texture(vec4(0, 0, 1, 1)), [](UIButton* button) {
			button->GetParent()->GetParent()->Kill();
		}));

		Actor* subpanelTest = new Actor(Transform(vec3(-135, -60, -2), vec3(1)));
		subpanelTest->AddComponent(new UIPanel(new Texture(vec4(1, 0, 0, 1))));
		auto l = new UILabel(new Font("res/Roboto.ttf"), "This is text.\nIt's really cool.\nIt's really not. B-)", 32.f);
		subpanelTest->AddComponent(l);
		subpanelTest->AddComponent(new UIDraggable(UIDragPosition::Centre));

		panelTest->AddChild(buttonTest);
		panelTest->AddChild(subpanelTest);

		actors.push_back(panelTest);
		actors.push_back(subpanelTest);
		actors.push_back(buttonTest);
	}

	renderer.testPart->Emit(1, vec3(-12, -4, -8));
	renderer.testPart->Update(renderer.GetCamera(), 1.f / 60.f);
}

#include <luabind/luabind.hpp>

i32 main(i32 argc, c8** argv) {
	SeedRandom();
	Window window("Bearish Engine 0.160a", 1280, 720);
	
	Timer timer;
	timer.Start();

	Assets::LoadAssetDefinitions();

	Scripting::InitLua();
	Scripting::RunFile("scr/lib/init.lua");

	Scripting::RegisterMath();
	Scripting::RegisterCore();
	Scripting::RegisterGraphics();

	Scripting::RunFile("scr/lib/class.lua");
	Scripting::InitMoonScript();


	for (auto& file : Util::GetFilesInFolder("./scr/run/", "lua")) {
		Logger::Info("Running %s", file.c_str());
		Scripting::RunFile(file);
	}

	for (auto& file : Util::GetFilesInFolder("./scr/run/", "moon")) {
		Logger::Info("Running %s", file.c_str());
		Scripting::DoMoonFile(file);
	}

	Scripting::RunString(R"(
for i, v in pairs(Key) do
	print(i, v)
end)");

	renderer.Load();


	i64 fps = 0;
	f32 secondTimer = 0;

	Mesh mesh = Model(Assets::Get("man")).ToMesh();
	Mesh mesh2 = Model(Assets::Get("plane")).ToMesh();
	
	Texture texture(Assets::Get("bricks"));
	Texture normalMap(Assets::Get("bricksNormal"));

	Texture texture2(Assets::Get("defaultNormal"));

	Mesh cp1(mesh);
	Mesh cp2(mesh);
	Mesh cp3(mesh);
	
	Shader* red = new Shader("res/phong.vert", "res/red.frag");
	object1->AddComponent(new AnimatedMeshRendererComponent("animation", 0.5f, &cp1, &texture2))->SetMaterial(new Material("red", red));
	object2->AddComponent(new AnimatedMeshRendererComponent("animation", 1.0f, &cp2, &texture2));
	object3->AddComponent(new AnimatedMeshRendererComponent("animation", 2.0f, &cp3, &texture2));
		   
	object3->GetTransform().Rotate(vec3(1, 0, 0), AsRadians(-90.f));
	object1->GetTransform().Rotate(vec3(1, 0, 0), AsRadians(-90.f));
	object2->GetTransform().Rotate(vec3(1, 0, 0), AsRadians(-90.f));

#if 0
	for (i32 i = 0; i < 10; i++) {
		for (i32 j = 0; j < 10; j++) {
			for (i32 k = 0; k < 5; k++) {
				Actor* a = new Actor(Transform(vec3(3 + i * 1.5f, k * 1.5f, 3 + j * 1.5f), vec3(0.1f)));
				a->GetTransform().Rotate(vec3(1, 0, 0), AsRadians(-90.f));
				a->AddComponent(new MeshRendererComponent(&mesh, &texture2));
				actors.push_back(a);
			}
		}
	}
#endif

	fpsCounter->AddComponent(new UILabel(new Font("res/Roboto.ttf"), "FPS: 0", 48));
	actors.push_back(fpsCounter);
	
	Actor* player = new Actor;
	player->AddComponent(new SkyboxComponent(new Texture(Assets::Get("right"), Assets::Get("left"), Assets::Get("top"), Assets::Get("bottom"), Assets::Get("front"), Assets::Get("back"))));

	//player->AddComponent(new CameraComponent(new Camera(mat4().CreateProjection(AsRadians(70.f), 16.f / 9.f, 0.1f, 1000))));
	player->AddComponent(new IActorComponent("CameraComponent"));

	//player->AddComponent(new FreeMoveComponent(1.0f / 3.0f));
	//player->AddComponent(new FreeLookComponent(0.2f));
	player->AddComponent(new IActorComponent("FreeLookComponent", 0.2f));
	player->AddComponent(new IActorComponent("FreeMoveComponent", 1.0f / 3.0f));
	Mouse::FreeFromCentre();

	Actor* plane = new Actor(Transform(vec3(0, -2.5f, 0), vec3(5, 1, 5)));
	plane->AddComponent(new MeshRendererComponent(&mesh2, &texture, &normalMap));

	Actor* dir = new Actor(Transform(vec3(0), vec3(1), quat().CreateRotation(vec3(1, 0, 0), AsRadians(45.f))));
	dir->AddComponent(new DirectionalLightComponent(vec3(1), 0.1f, 0.2f));

	Actor* point1 = new Actor(Transform(vec3(2, -1.5f, 0)));
	point1->AddComponent(new PointLightComponent(vec3(1, 1, 0), 0, Attenuation(0, 0, 1), 2));

	Actor* point2 = new Actor(Transform(vec3(-2, -1.5f, 0)));
	point2->AddComponent(new PointLightComponent(vec3(1, 0, 0), 0, Attenuation(0, 0, 0.85f), 5));

	actors.push_back(player);
	actors.push_back(plane);
	actors.push_back(object1);

	actors.push_back(object2);
	actors.push_back(object3);
	actors.push_back(dir);
	actors.push_back(point1);
	actors.push_back(point2);
	
	renderer.SetActorReference(&actors);
	renderer.SetWindow(&window);

	window.SetVsync(0);

	i64 f = 0;
	f64 updateTimer = 0;

	Logger::Info("Initial loading took %.1fms.", timer.DeltaMS());
	
	while (!window.ShouldClose()) {
		while (updateTimer > UPDATE_TIME) {
			Keyboard::Update();
			Mouse::Update();
			Update();
			updateTimer -= UPDATE_TIME;
		}

		for (auto& a : actors) {
			a->Update();
		}

		renderer.Draw();
		window.Update();

		fps++;

		f32 delta = timer.LoopMS();
		updateTimer += delta / 1000.f;
		secondTimer += delta / 1000.f;

		if (secondTimer >= 1) {
			secondTimer = 0;
			f32 d = (f32)(1.f / (f32)fps) * 1000.f;
			fpsCounter->GetComponentsByType<UILabel>().at(0)->SetText("FPS: " + std::to_string(fps), 48);
			Logger::Info("FPS: %d", fps);
			Logger::Info("Drawtime: %.3fms", d);
			fps = 0;
		}
	}

	//KillSquirrel();
	return (i32)null;
}

///////
// #SCREENSAVERJAM
//////////
/*
std::vector<Actor*> actors;
RenderingEngine renderer;
bool fs = false;
bool running = true;
// Temporary update solution.
void Update() {
	for (i32 i = 0; i < actors.size(); i++) {
		if (actors.at(i)->IsDead()) {
			Actor* todel = actors.at(i);
			actors.insert(actors.begin() + i, actors.at(actors.size() - 1));
			actors.resize(actors.size() - 1);
			delete todel;
		}
	}

	for (auto& a : actors) {
		a->FixedUpdate();
	}

	if (Keyboard::IsKeyPressed(Key::F1)) {
		Renderer::SetFillMode(FillMode::Normal);
	}

	if (Keyboard::IsKeyPressed(Key::F2)) {
		Renderer::SetFillMode(FillMode::Wireframe);
	}

	if (Keyboard::IsKeyPressed(Key::F3)) {
		Renderer::SetFillMode(FillMode::Points);
	}

	if (Keyboard::IsKeyPressed(Key::F4)) {
		renderer.SetDebugDeffered(false);
	}

	if (Keyboard::IsKeyPressed(Key::F5)) {
		renderer.SetDebugDeffered(true);
	}

	if (Keyboard::IsKeyPressed(Key::Q)) {
		running = false;
	}

	renderer.testPart->Emit(4, vec3(-12.5f, -6, 15));
	renderer.testPart->Update(renderer.GetCamera(), 1.f / 60.f);
}

i32 main(i32 argc, c8** argv) {
	bool fs = false;
	if (argc > 1 && strcmp(argv[1], "-f") == 0) fs = true;
	
	Window window("Bearish Engine 0.160a", 640, 360, fs);
	SeedRandom();

	Timer timer;
	timer.Start();

	renderer.Load();

	i64 fps = 0;
	f32 secondTimer = 0;

	Actor* player = new Actor;
	player->AddComponent(new CameraComponent(new Camera(mat4().CreateProjection(AsRadians(24.f), 16.f / 9.f, 0.1f, 1000))));
	actors.push_back(player);

	renderer.SetActorReference(&actors);
	renderer.SetWindow(&window);

	window.SetVsync(1);
	

	i64 f = 0;
	f64 updateTimer = 0;

	Logger::Info("Initial loading took %.1fms.", timer.DeltaMS());

	while (!window.ShouldClose() && running) {
		while (updateTimer > UPDATE_TIME) {
			Keyboard::Update();
			Mouse::Update();
			Update();
			updateTimer -= UPDATE_TIME;
		}

		for (auto& a : actors) {
			a->Update();
		}

		renderer.Draw();
		window.Update();

		fps++;

		f32 delta = timer.LoopMS();
		updateTimer += delta / 1000.f;
		secondTimer += delta / 1000.f;

		if (secondTimer >= 1) {
			secondTimer = 0;
			f32 d = (f32)(1.f / (f32)fps) * 1000.f;
			Logger::Info("FPS: %d", fps);
			Logger::Info("Drawtime: %.3fms", d);
			fps = 0;
		}
	}

	return (i32)null;
}*/