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

Actor* player;

Actor* fpsCounter = new Actor(Transform(vec3(10, 40, -2)));

std::vector<Actor*> actors;
RenderingEngine renderer;

std::unordered_map<string, string> Asset::_values;

Font* UI_FONT;

Actor* UI_PANEL;
Actor* UI_GRAPH;

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
			i--;
			continue;
		}

		actors.at(i)->FixedUpdate();
	}

	if (Keyboard::IsKeyPressed(Key::U)) {
		Logger::Info(player->GetTransform().GetTranslation().ToString());
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
			UI_GRAPH = new Actor(Transform(vec3(1920 - 150, 400, -1), vec3(300, 800, 1)));
			UI_GRAPH->AddComponent(new UIPanel(new Texture2D(vec4(0.2, 0.2, 0.2, 0.65))));

			Actor* graph = new Actor(Transform(vec3(0, -300, 0), vec3(1, 0.25, 1)));
			graph->AddComponent(new UILineGraph(0, 60, 60));
			UI_GRAPH->AddChild(graph);

			Actor* graph2 = new Actor(Transform(vec3(0, -100, 0), vec3(1, 0.25, 1)));
			graph2->AddComponent(new UILineGraph(0, 60, 60, vec4(1, 0.4, 0.4, 1)));
			UI_GRAPH->AddChild(graph2);

			Actor* graph3 = new Actor(Transform(vec3(0, 100, 0), vec3(1, 0.25, 1)));
			graph3->AddComponent(new UILineGraph(0, 60, 60, vec4(0.4, 0.4, 1, 1)));
			UI_GRAPH->AddChild(graph3);

			Actor* graph4 = new Actor(Transform(vec3(0, 300, 0), vec3(1, 0.25, 1)));
			graph4->AddComponent(new UILineGraph(0, 60, 60, vec4(1, 0.4, 1, 1)));
			UI_GRAPH->AddChild(graph4);

			Actor* glb1 = new Actor(Transform(vec3(-150, -386, 0)));
			glb1->AddComponent(new IActorComponent("UILabel", UI_FONT, "60ms", 18));
			UI_GRAPH->AddChild(glb1);

			Actor* glb2 = new Actor(Transform(vec3(-150, -200, 0)));
			glb2->AddComponent(new IActorComponent("UILabel", UI_FONT, "0ms", 18));
			UI_GRAPH->AddChild(glb2);

			Actor* glb3 = new Actor(Transform(vec3(-150, -186, 0)));
			glb3->AddComponent(new IActorComponent("UILabel", UI_FONT, "60ms", 18));
			UI_GRAPH->AddChild(glb3);

			Actor* glb4 = new Actor(Transform(vec3(-150, 0, 0)));
			glb4->AddComponent(new IActorComponent("UILabel", UI_FONT, "0ms", 18));
			UI_GRAPH->AddChild(glb4);

			Actor* glb5 = new Actor(Transform(vec3(-150, 14, 0)));
			glb5->AddComponent(new IActorComponent("UILabel", UI_FONT, "60ms", 18));
			UI_GRAPH->AddChild(glb5);

			Actor* glb6 = new Actor(Transform(vec3(-150, 200, 0)));
			glb6->AddComponent(new IActorComponent("UILabel", UI_FONT, "0ms", 18));
			UI_GRAPH->AddChild(glb6);

			Actor* glb7 = new Actor(Transform(vec3(-150, 214, 0)));
			glb7->AddComponent(new IActorComponent("UILabel", UI_FONT, "60ms", 18));
			UI_GRAPH->AddChild(glb7);

			Actor* glb8 = new Actor(Transform(vec3(-150, 400, 0)));
			glb8->AddComponent(new IActorComponent("UILabel", UI_FONT, "0ms", 18));
			UI_GRAPH->AddChild(glb8);

			UI_PANEL = new Actor(Transform(vec3(150, 540, -1), vec3(300, 1080, 1)));
			UI_PANEL->AddComponent(new UIPanel(new Texture2D(vec4(0.2, 0.2, 0.2, 0.5))));

			Actor* button1 = new Actor(Transform(vec3(0, -440, 1), vec3(260, 75, 1)));
			button1->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(0);
			}));
			Actor* label1 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
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
			label3->AddComponent(new IActorComponent("UILabel", UI_FONT, "Specular", 48));
			button3->AddChild(label3);
			UI_PANEL->AddChild(button3);

			Actor* button4 = new Actor(Transform(vec3(0, -140, 1), vec3(260, 75, 1)));
			button4->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(3);
			}));
			Actor* label4 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			label4->AddComponent(new IActorComponent("UILabel", UI_FONT, "Diffuse", 48));
			button4->AddChild(label4);
			UI_PANEL->AddChild(button4);

			Actor* button5 = new Actor(Transform(vec3(0, -40, 1), vec3(260, 75, 1)));
			button5->AddComponent(new UIButton(new Texture2D(vec4(0.15, 0.15, 0.15, 0.7)), [](UIButton* self) {
				renderer.SetDebugMode(2);
			}));
			Actor* label5 = new Actor(Transform(vec3(-120, 20, 10), vec3(1, 1, 1)));
			label5->AddComponent(new IActorComponent("UILabel", UI_FONT, "Normal XY", 48));
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
			actors.push_back(UI_GRAPH);
			PANEL_OPEN = true;
		}
		else {
			PANEL_OPEN = false;
			UI_PANEL->Kill();
			UI_GRAPH->Kill();
		}
	}

	renderer.testPart->Emit(1, vec3(-12, -4, -8));
	renderer.testPart->Update(renderer.GetCamera(), 1.f / 60.f);
}

i32 main(i32 argc, c8** argv) {
	GUI::Win32Window testwin("TEST", 1280, 720);
	
	GUI::Win32WindowMenuBar menu;
	
	GUI::Win32WindowSubMenu file("&File");
	file.AddChild(new GUI::Win32WindowSubMenuItem("&Do nothing", []() {}));
	file.AddChild(new GUI::Win32WindowSubMenuItem("&Close", [&testwin]() { testwin.Close(); }));
	menu.AddChild(&file);

	GUI::Win32WindowSubMenu edit("&Edit");
	edit.AddChild(new GUI::Win32WindowSubMenuItem("&This is an option", []() {}));
	edit.AddChild(new GUI::Win32WindowSubMenuItem("&Olle suger", []() {}));
	menu.AddChild(&edit);

	testwin.AddComponent(&menu);

	testwin.AddComponent(new GUI::Win32WindowGLViewport(0, 0, 1280, 720));
	testwin.Open();

	PANEL_OPEN = false;
	SeedRandom();
	
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

	TextureCube* skybox = new TextureCube(Asset::Get("right"), Asset::Get("left"), Asset::Get("top"),
											Asset::Get("bottom"), Asset::Get("front"), Asset::Get("back"));

	Mesh cp1(mesh);
	Mesh cp2(mesh);
	Mesh cp3(mesh);
	
	Material dude("Dude", new Shader("res/phong.vert", "res/phong.frag"));
	dude.Set("diffuse", &texture2, 5);
	dude.Set("normalMap", &texture2, 6);

	Material red("red", new Shader("res/phong.vert", "res/red.frag"));

	Material brickMaterial("Bricks", new Shader("res/phong.vert", "res/phong.frag"));
	brickMaterial.Set("diffuse", &texture, 5);
	brickMaterial.Set("normalMap", &normalMap, 6);

	Material pbrTest("PBR", new Shader("res/PBR.vert", "res/PBR.frag"));
	pbrTest.Set("AlbedoColor", vec4(0.7, 0.7, 0.7, 1.0f));
	pbrTest.Set("SpecularColor", vec3(0.2f));
	pbrTest.Set("GlossColor", 0.25f);
	pbrTest.Set("NormalMap", &texture2, 10);

	renderer.SetPreFG(new Texture2D(Asset::Get("PreFG")));
	renderer.SetEnvironmentMap(skybox);

	object1->AddComponent(new AnimatedMeshRendererComponent("animation", 0.5f, &cp1, &pbrTest));
	object2->AddComponent(new AnimatedMeshRendererComponent("animation", 1.0f, &cp2, &pbrTest));
	object3->AddComponent(new AnimatedMeshRendererComponent("animation", 2.0f, &cp3, &pbrTest));
		   
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
	
	player = new Actor;
	player->GetTransform().Rotate(vec3(1, 1, 0), 0.4);
	player->AddComponent(new SkyboxComponent(skybox));

	player->AddComponent(new IActorComponent("CameraComponent"));

	//player->AddComponent(new IActorComponent("EditorLookComponent"));
	player->AddComponent(new IActorComponent("FreeLookComponent", 0.2f));
	player->AddComponent(new IActorComponent("FreeMoveComponent", 1.0f / 3.0f));
	Mouse::FreeFromCentre();

	for (auto x = 0; x < 10; x++) {
		Material* mat = new Material(pbrTest);
		f32 xx = x * 10.0f;

		f32 roughness = x / 10.0f;
		vec3 spec(1.0f);
		vec4 diffuse(0.0f, 0.0f, 0.0f, 1.0f);

		mat->Set("AlbedoColor", diffuse);
		mat->Set("SpecularColor", spec);
		mat->Set("GlossColor", 1.0f - roughness);

		for (auto y = 0; y < 10; y++) {
			Actor* plane = new Actor(Transform(vec3(x * 10.f, -2.5f, y * 10.f), vec3(5.f, 1.f, 5.f)));

			plane->AddComponent(new MeshRendererComponent(&mesh2, &brickMaterial));
			//actors.push_back(plane);
		}
	}

	Actor* dir = new Actor(Transform(vec3(0.f), vec3(1.f), quat().CreateRotation(vec3(1.f, 0.f, 0.f), AsRadians(83.f))));
	dir->AddComponent(new DirectionalLightComponent(vec3(1.f), 0.1f, 0.2f));

	Actor* ples = new Actor(Transform(vec3(2.f, 15.f, 0.0f)));
	ples->AddComponent(new PointLightComponent(vec3(1.f), 0.f, Attenuation(0.f, 0.f, 0.0055f), 0.3f));
	actors.push_back(ples);

	actors.push_back(player);
	//actors.push_back(object1);
	//
	//actors.push_back(object2);
	//actors.push_back(object3);
	actors.push_back(dir);

#if 1
	Material* thornMaterial = new Material(pbrTest);
	thornMaterial->Set("UsingAlbedoMap", 1.f);
	thornMaterial->Set("UsingSpecularMap", 1.f);
	thornMaterial->Set("AlbedoMap", new Texture("asset/sponza2/thorn.bet"), 7);
	thornMaterial->Set("SpecularMap", new Texture("asset/sponza2/thorn_spec.bet"), 8);
	thornMaterial->Set("NormalMap", new Texture("asset/sponza2/thorn_bump.bet"), 10);

	Material* colAMaterial = new Material(pbrTest);
	colAMaterial->Set("UsingAlbedoMap", 1.f);
	colAMaterial->Set("UsingSpecularMap", 1.f);
	colAMaterial->Set("AlbedoMap", new Texture("asset/sponza2/cola.bet"), 7);
	colAMaterial->Set("SpecularMap", new Texture("asset/sponza2/cola_spec.bet"), 8);
	colAMaterial->Set("NormalMap", new Texture("asset/sponza2/cola_bump.bet"), 10);

	Material* colBMaterial = new Material(pbrTest);
	colBMaterial->Set("UsingAlbedoMap", 1.f);
	colBMaterial->Set("UsingSpecularMap", 1.f);
	colBMaterial->Set("AlbedoMap", new Texture("asset/sponza2/colb.bet"), 7);
	colBMaterial->Set("SpecularMap", new Texture("asset/sponza2/colb_spec.bet"), 8);
	colBMaterial->Set("NormalMap", new Texture("asset/sponza2/colb_bump.bet"), 10);

	Material* colCMaterial = new Material(pbrTest);
	colCMaterial->Set("UsingAlbedoMap", 1.f);
	colCMaterial->Set("UsingSpecularMap", 1.f);
	colCMaterial->Set("AlbedoMap", new Texture("asset/sponza2/colc.bet"), 7);
	colCMaterial->Set("SpecularMap", new Texture("asset/sponza2/colc_spec.bet"), 8);
	colCMaterial->Set("NormalMap", new Texture("asset/sponza2/colc_bump.bet"), 10);

	Material* ceilAMaterial = new Material(pbrTest);
	ceilAMaterial->Set("UsingAlbedoMap", 1.f);
	ceilAMaterial->Set("UsingSpecularMap", 1.f);
	ceilAMaterial->Set("AlbedoMap", new Texture("asset/sponza2/ceila.bet"), 7);
	ceilAMaterial->Set("SpecularMap", new Texture("asset/sponza2/ceila_spec.bet"), 8);

	Material* archMaterial = new Material(pbrTest);
	archMaterial->Set("UsingAlbedoMap", 1.f);
	archMaterial->Set("UsingSpecularMap", 1.f);
	archMaterial->Set("AlbedoMap", new Texture("asset/sponza2/arch.bet"), 7);
	archMaterial->Set("SpecularMap", new Texture("asset/sponza2/arch_spec.bet"), 8);
	archMaterial->Set("NormalMap", new Texture("asset/sponza2/arch_bump.bet"), 10);

	Material* bricksAMaterial = new Material(pbrTest);
	bricksAMaterial->Set("UsingAlbedoMap", 1.f);
	bricksAMaterial->Set("UsingSpecularMap", 1.f);
	bricksAMaterial->Set("AlbedoMap", new Texture("asset/sponza2/bricksa.bet"), 7);
	bricksAMaterial->Set("SpecularMap", new Texture("asset/sponza2/bricksa_spec.bet"), 8);
	bricksAMaterial->Set("NormalMap", new Texture("asset/sponza2/bricksa_bump.bet"), 10);

	Material* floorAMaterial = new Material(pbrTest);
	floorAMaterial->Set("UsingAlbedoMap", 1.f);
	floorAMaterial->Set("UsingSpecularMap", 1.f);
	floorAMaterial->Set("AlbedoMap", new Texture("asset/sponza2/floora.bet"), 7);
	floorAMaterial->Set("SpecularMap", new Texture("asset/sponza2/floora_spec.bet"), 8);

	Material* detailMaterial = new Material(pbrTest);
	detailMaterial->Set("UsingAlbedoMap", 1.f);
	detailMaterial->Set("UsingSpecularMap", 1.f);
	detailMaterial->Set("AlbedoMap", new Texture("asset/sponza2/details.bet"), 7);
	detailMaterial->Set("SpecularMap", new Texture("asset/sponza2/details_spec.bet"), 8);

	Material* vaseMaterial = new Material(pbrTest);
	vaseMaterial->Set("UsingAlbedoMap", 1.f);
	vaseMaterial->Set("AlbedoMap", new Texture("asset/sponza2/vase.bet"), 7);

	Material* chainMaterial = new Material(pbrTest);
	chainMaterial->Set("UsingAlbedoMap", 1.f);
	chainMaterial->Set("AlbedoMap", new Texture("asset/sponza2/chain.bet"), 7);
	chainMaterial->Set("NormalMap", new Texture("asset/sponza2/chain_bump.bet"), 10);

	Material* backgroundMaterial = new Material(pbrTest);
	backgroundMaterial->Set("UsingAlbedoMap", 1.f);
	backgroundMaterial->Set("AlbedoMap", new Texture("asset/sponza2/background.bet"), 7);
	backgroundMaterial->Set("NormalMap", new Texture("asset/sponza2/background_bump.bet"), 10);

	Material* lionMaterial = new Material(pbrTest);
	lionMaterial->Set("UsingAlbedoMap", 1.f);
	lionMaterial->Set("AlbedoMap", new Texture("asset/sponza2/lion.bet"), 7);
	lionMaterial->Set("NormalMap", new Texture("asset/sponza2/lion_bump.bet"), 10);

	Material* flagpoleMaterial = new Material(pbrTest);
	flagpoleMaterial->Set("UsingAlbedoMap", 1.f);
	flagpoleMaterial->Set("UsingSpecularMap", 1.f);
	flagpoleMaterial->Set("AlbedoMap", new Texture("asset/sponza2/flagpole.bet"), 7);
	flagpoleMaterial->Set("SpecularMap", new Texture("asset/sponza2/flagpole_spec.bet"), 8);

	Material* roofMaterial = new Material(pbrTest);
	roofMaterial->Set("UsingAlbedoMap", 1.f);
	roofMaterial->Set("AlbedoMap", new Texture("asset/sponza2/roof.bet"), 7);

	Material* curtRedMaterial = new Material(pbrTest);
	curtRedMaterial->Set("UsingAlbedoMap", 1.f);
	curtRedMaterial->Set("AlbedoMap", new Texture("asset/miko.bet"/*"asset/sponza2/curtain.bet"*/), 7);

	Material* curtBlueMaterial = new Material(pbrTest);
	curtBlueMaterial->Set("UsingAlbedoMap", 1.f);
	curtBlueMaterial->Set("AlbedoMap", new Texture("asset/sponza2/curtain_blue.bet"), 7);

	Material* curtGreenMaterial = new Material(pbrTest);
	curtGreenMaterial->Set("UsingAlbedoMap", 1.f);
	curtGreenMaterial->Set("AlbedoMap", new Texture("asset/sponza2/curtain_green.bet"), 7);

	Material* fabRedMaterial = new Material(pbrTest);
	fabRedMaterial->Set("UsingAlbedoMap", 1.f);
	fabRedMaterial->Set("UsingSpecularMap", 1.f);
	fabRedMaterial->Set("AlbedoMap", new Texture("asset/sponza2/fabric.bet"), 7);
	fabRedMaterial->Set("SpecularMap", new Texture("asset/sponza2/fabric_spec.bet"), 8);

	Material* fabBlueMaterial = new Material(pbrTest);	
	fabBlueMaterial->Set("UsingAlbedoMap", 1.f);
	fabBlueMaterial->Set("UsingSpecularMap", 1.f);
	fabBlueMaterial->Set("AlbedoMap", new Texture("asset/sponza2/fabric_blue.bet"), 7);
	fabBlueMaterial->Set("SpecularMap", new Texture("asset/sponza2/fabric_spec.bet"), 8);

	Material* fabGreenMaterial = new Material(pbrTest);
	fabGreenMaterial->Set("UsingAlbedoMap", 1.f);
	fabGreenMaterial->Set("UsingSpecularMap", 1.f);
	fabGreenMaterial->Set("AlbedoMap", new Texture("asset/sponza2/fabric_green.bet"), 7);
	fabGreenMaterial->Set("SpecularMap", new Texture("asset/sponza2/fabric_spec.bet"), 8);


	std::vector<std::pair<std::vector<i32>, Material*>> materials = {
		{{0, 2, 45, 145, 146, 333, 370, 371, 44, 95, 96, 97, 208, 209, 216, 217}, thornMaterial},
		{{42, 82, 83, 137, 138, 139, 140, 141, 143, 144, 374, 375}, colAMaterial},
		{{1, 3, 14, 15, 19, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 130, 131, 132, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 259, 260, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 299, 300, 301, 302, 303, 304 ,305, 306, 307, 308, 321, 322, 323, 324, 325, 326, 327, 328, 335, 336, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 372, 373, 378, 380, 381}, colBMaterial},
		{{16, 17, 20, 21, 84, 85, 88, 89, 90, 91, 94, 98, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 192, 193, 194, 195, 196, 197, 198, 199, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 261, 262, 263, 264, 265, 266, 267, 268, 360, 361, 362, 363}, colCMaterial},
		{{43, 73, 75, 77, 79, 81, 129, 136, 182, 184, 186, 365, 368}, ceilAMaterial},
		{{26, 49, 72, 74, 76, 78, 80, 142, 180, 181, 183, 185, 187, 188, 189, 269, 270, 311, 312, 313, 314, 315, 317, 318, 367, 369, 376, 377, 379}, archMaterial},
		{{4, 5, 10, 11, 13, 55, 56, 57, 58, 60, 178, 179, 190, 191, 215, 292}, chainMaterial},
		{{6, 7, 8, 9, 12, 51, 52, 53, 54, 59, 210, 211, 212, 213, 214, 291, 293, 294, 295, 296, 99, 100, 101, 339}, vaseMaterial},
		{{47, 50, 86, 104, 134, 161, 258, 309, 310, 316, 364, 366}, bricksAMaterial},
		{{87, 135}, floorAMaterial},
		{{18, 22, 23, 24, 25}, detailMaterial},
		{{46}, backgroundMaterial},
		{{102, 103}, lionMaterial},
		{{92, 93, 133, 200, 201, 202, 203, 204, 205, 206, 207, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 283, 284, 285, 285, 287, 288, 289, 290, 297, 298, 329, 330, 331, 332, 334, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349}, flagpoleMaterial},
		{{256, 257}, roofMaterial},
		{{148, 153, 155, 159}, curtRedMaterial},
		{{147, 156, 158}, curtBlueMaterial},
		{{157, 160, 338}, curtGreenMaterial},
		{{149, 337}, fabRedMaterial},
		{{150, 152, 319}, fabGreenMaterial},
		{{151, 154, 320}, fabBlueMaterial}
	};

	for (i32 i = 0; i < 393; i++) {
		auto model = Model("asset/sponza2/sponza_" + std::to_string(i) + ".bem");
		Actor* a = new Actor(Transform(vec3(0), vec3(0.01)));
		Material* mat = &pbrTest;

		if (model.name.find("Mesh", 0) == string::npos) {
			mat = thornMaterial;
		}
		else {
			for (auto& material : materials) {
				for (auto& num : material.first) {
					if (model.name == std::string("Mesh") + (num == 0 ? "" : "_") + 
						(num > 0 && num < 100 ? "0" : "") +
						(num > 0 && num < 10 ? "0" : "") + std::to_string(num))
						mat = material.second;
				}
			}
		}

		a->AddComponent(new MeshRendererComponent(new Mesh(model), mat));
		actors.push_back(a);
	}
#elif 1
for (i32 i = 0; i < 1087; i++) {
	auto model = Model("asset/sibenik/sibenik_" + std::to_string(i) + ".bem");
	Actor* a = new Actor(Transform(vec3(0), vec3(1)));
	Material* mat = &pbrTest;

	a->AddComponent(new MeshRendererComponent(new Mesh(model), mat));
	actors.push_back(a);
}
#else
for (i32 i = 0; i < 41; i++) {
	auto model = Model("asset/conference/conference_" + std::to_string(i) + ".bem");
	Actor* a = new Actor(Transform(vec3(0), vec3(0.01)));
	Material* mat = &pbrTest;

	a->AddComponent(new MeshRendererComponent(new Mesh(model), mat));
	actors.push_back(a);
}
#endif

	renderer.SetActorReference(&actors);
	//renderer.SetWindow(&window);

	//window.SetVsync(0);

	i64 f = 0;
	f64 updateTimer = 0;

	Logger::Info("Initial loading took %.1fms.", timer.DeltaMS());
	
	Timer lastFrameTimer;
	Timer lastDrawTimer;
	Timer lastUpdateTimer;
	lastFrameTimer.Start();
	lastDrawTimer.Start();
	lastUpdateTimer.Start();
	f32 lastFrameTime = 0;
	f32 lastDrawTime = 0;
	f32 lastUpdateTime = 0;
	bool updated = false;

	while (testwin.IsOpen()) {
		updated = false;
		lastFrameTimer.LoopMS();
		while (updateTimer > UPDATE_TIME) {
			Keyboard::Update();
			Mouse::Update();
			Update();
			updateTimer -= UPDATE_TIME;

			if (PANEL_OPEN) {
				UI_GRAPH->GetChildren()[0]->GetComponentsByType<UILineGraph>()[0]->AddDataPoint(lastFrameTime);
				UI_GRAPH->GetChildren()[1]->GetComponentsByType<UILineGraph>()[0]->AddDataPoint(lastUpdateTime);
				UI_GRAPH->GetChildren()[2]->GetComponentsByType<UILineGraph>()[0]->AddDataPoint(lastDrawTime);
				UI_GRAPH->GetChildren()[3]->GetComponentsByType<UILineGraph>()[0]->AddDataPoint(renderer.GetCPUTime());
			}
		
			updated = true;
			//Shader::ReloadChanged();
		}

		lastUpdateTimer.LoopMS();

		for (auto& a : actors) {
			a->Update();
		}

		if (updated) {
			lastUpdateTime = lastUpdateTimer.LoopMS();
		}

		lastDrawTimer.LoopMS();
		renderer.Draw();
		glFlush();

		if (updated) {
			lastFrameTime = lastFrameTimer.LoopMS();
			lastDrawTime = lastDrawTimer.LoopMS();
		}

		testwin.Update();


		fps++;

		f32 delta = timer.LoopMS();
		updateTimer += delta / 1000.f;
		secondTimer += delta / 1000.f;

		if (secondTimer >= 1) {
			secondTimer = 0;
			f32 d = (f32)(1.f / (f32)fps) * 1000.f;
			fpsCounter->GetComponentsByType<UILabel>().at(0)->SetText("FPS: " + std::to_string(fps), 48);
			//Scripting::UpdateScripts();
			fps = 0;
		}
		
	}

	//Scripting::KillLua();
	return (i32)null;
}