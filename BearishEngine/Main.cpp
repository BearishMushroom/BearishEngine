#include "src/Bearish.h"
#include <vector>

using namespace Bearish;

using namespace Components;
using namespace Graphics;
using namespace Core;
using namespace Math;
using namespace Util;

#define UPDATE_TIME 1.0 / 60.0

Actor* object1 = new Actor(Transform(vec3( 0,    0, 0), vec3(0.1f)));
Actor* object2 = new Actor(Transform(vec3( 1.5f, 0, 0), vec3(0.1f)));
Actor* object3 = new Actor(Transform(vec3(-1.5f, 0, 0), vec3(0.1f)));

Actor* fpsCounter = new Actor(Transform(vec3(10, 40, -2)));

std::vector<Actor*> actors;
RenderingEngine renderer;

std::unordered_map<string, string> Asset::_values;

Font* UI_FONT;
Actor* UI_PANEL;
bool PANEL_OPEN;

// Temporary update solution.
void Update() {
	for (i32 i = 0; i < (i32)actors.size(); i++) {
		if (actors.at(i)->IsDead()) {
			Actor* todel = actors.at(i);

			if (i < (i32)actors.size() - 1) {
				actors.insert(actors.begin() + i, actors.at(actors.size() - 1));
			}

			actors.resize(actors.size() - 1);
			delete todel;
			todel = nullptr;
		}

		actors.at(i)->FixedUpdate();
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
		// DEBUG MENU CREATION!!!?!
		if (!PANEL_OPEN) {
			UI_PANEL = new Actor(Transform(vec3(150, 540, -1), vec3(300, 1080, 1)));
			UI_PANEL->AddComponent(new UIPanel(new Texture2D(vec4(0.2, 0.2, 0.2, 0.5))));

			Actor* button1 = new Actor(Transform(vec3(0, -440, 1), vec3(260, 75, 1)));
			button1->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(0);
			}));
			Actor* label1 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			//label1->AddComponent(new UILabel(UI_FONT, "Full render", 48));
			label1->AddComponent(new IActorComponent("UILabel", UI_FONT, "Full render", 48));
			button1->AddChild(label1);
			UI_PANEL->AddChild(button1);

			Actor* button2 = new Actor(Transform(vec3(0, -340, 1), vec3(260, 75, 1)));
			button2->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(1);
			}));
			Actor* label2 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			label2->AddComponent(new IActorComponent("UILabel", UI_FONT, "World", 48));
			button2->AddChild(label2);
			UI_PANEL->AddChild(button2);

			Actor* button3 = new Actor(Transform(vec3(0, -240, 1), vec3(260, 75, 1)));
			button3->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(4);
			}));
			Actor* label3 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			label3->AddComponent(new IActorComponent("UILabel", UI_FONT, "Tex + Mat", 48));
			button3->AddChild(label3);
			UI_PANEL->AddChild(button3);

			Actor* button4 = new Actor(Transform(vec3(0, -140, 1), vec3(260, 75, 1)));
			button4->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(3);
			}));
			Actor* label4 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			label4->AddComponent(new IActorComponent("UILabel", UI_FONT, "Normal", 48));
			button4->AddChild(label4);
			UI_PANEL->AddChild(button4);

			Actor* button5 = new Actor(Transform(vec3(0, -40, 1), vec3(260, 75, 1)));
			button5->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(2);
			}));
			Actor* label5 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			label5->AddComponent(new IActorComponent("UILabel", UI_FONT, "Tangent", 48));
			button5->AddChild(label5);
			UI_PANEL->AddChild(button5);

			Actor* button6 = new Actor(Transform(vec3(0, 60, 1), vec3(260, 75, 1)));
			button6->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(5);
			}));
			Actor* label6 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			label6->AddComponent(new IActorComponent("UILabel", UI_FONT, "Shadow", 48));
			button6->AddChild(label6);
			UI_PANEL->AddChild(button6);

			actors.push_back(UI_PANEL);
			PANEL_OPEN = true;
		}
		else {
			PANEL_OPEN = false;
			UI_PANEL->Kill();
		}
	}

	renderer.testPart->Emit(1, vec3(-12, -4, -8));
	renderer.testPart->Update(renderer.GetCamera(), 1.f / 60.f);
}

i32 main(i32 argc, c8** argv) {
	PANEL_OPEN = false;
	SeedRandom();
	Window window("Bearish Engine 0.184a", 1280, 720);
	
	Timer timer;
	timer.Start();

	Asset::LoadAssetDefinitions();

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

	renderer.Load();

	i64 fps = 0;
	f32 secondTimer = 0;

	UI_FONT = new Font("res/Roboto.ttf");

	Mesh mesh = Model(Asset::Get("man")).ToMesh();
	Mesh mesh2 = Model(Asset::Get("plane")).ToMesh();
	
	Texture2D texture(Asset::Get("bricks"));
	Texture2D normalMap(Asset::Get("bricksNormal"));

	Texture2D texture2(Asset::Get("defaultNormal"));

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
	player->AddComponent(new SkyboxComponent(new Texture(Asset::Get("right"), Asset::Get("left"), Asset::Get("top"), Asset::Get("bottom"), Asset::Get("front"), Asset::Get("back"))));

	player->AddComponent(new IActorComponent("CameraComponent"));

	//player->AddComponent(new IActorComponent("EditorLookComponent"));
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
			//Shader::ReloadChanged();
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
			//Scripting::UpdateScripts();
			Logger::Info("FPS: %d", fps);
			Logger::Info("Drawtime: %.3fms", d);
			fps = 0;
		}
	}

	//Scripting::KillLua();
	return (i32)null;
}