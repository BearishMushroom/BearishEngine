#include "RenderingEngine.h"

#include <BE/Core/Transform.h>
#include <BE/Core/Asset.h>
#include <BE/Core/Actor.h>
#include <BE/Core/Model.h>
#include <BE/Core/Timer.h>
#include <BE/Core/Settings.h>

#include "Renderer.h"
#include "Texture/Texture.h"
#include "Camera.h"
#include "Lights.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh/Mesh.h"

#include <BE/Utils.h>
#include <BE/Components/UITweakBar.h>

#include <random>
#include <numeric>

using namespace Bearish;
using namespace Core;
using namespace Graphics;
using namespace Math;
using namespace Components;

f32 timef = 0;

RenderingEngine::RenderingEngine() {
	_timer.Start();
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::Load() {
	OpenGL::Init();
	_renderer = new Renderer();

	_shadowShader = new Shader("res/shadowmap.vert", "res/shadowmap.frag");
	_guiShader = new Shader("res/gui.vert", "res/gui.frag");
	_blur7 = new Shader("res/blur7.vert", "res/blur7.frag");
	_blur9 = new Shader("res/blur9.vert", "res/blur9.frag");
	_blur = 0;


	std::vector<TextureAttachment> att = {
		TextureAttachment::Color0,
		TextureAttachment::Color1,
		TextureAttachment::Color2,
		TextureAttachment::Color3,
	};

	// Gbuffer at 720p:
	/*
		Position buffer     : 1280 * 720 * (RGBA32)16 = 14.1MB
		Normal buffer       : 1280 * 720 * (RG16)   4 = 3.5MB
		Diffuse buffer      : 1280 * 720 * (RGB8)   3 = 2.6MB
		Spec + gloss buffer : 1280 * 720 * (RGBA8)  4 = 3.5MB
		Total               : 14.1 + 3.5 + 2.6 + 3.5 = 23.7MB of VRAM
		Total BPP           : 27 * 8 = 216 BPP
	*/

	// Shadowmap size at "high":
	/*
		Map0  : 2048 * 2048 * (RG32)8 = 32MB
		Map1  : 2048 * 2048 * (RG32)8 = 32MB
		Map2  : 2048 * 2048 * (RG32)8 = 32MB
		Total : 32 + 32 + 32 = 96MB of VRAM
	*/
	
	std::vector<TextureFormat> fmt {
		TextureFormat::RGBA32, // Position
		TextureFormat::RGB16,  // Normal XY
		TextureFormat::RGB,   // Diffuse
		TextureFormat::RGBA,  // Spec + gloss
	};


	_gbuffer = new Texture(vec2i(Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y")), TextureType::Texture2D, att, fmt, 4);
	_ssaoBuffer = new Texture(vec2i(Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y")), TextureType::Texture2D, TextureFilter::Nearest, TextureAttachment::Color0, TextureFormat::R, 0);

	_geomShader = new Shader("res/geometrypass.vert", "res/geometrypass.frag");
	_geomShader->SetName("geom");
	_pbrShader = new Shader("res/PBR.vert", "res/PBR.frag");
	_sphere = new Mesh(Model("asset/sphere.bem").ToMesh());

	// 2x scale and reversed winding because NDC is 1 - -1 and not -0.5 - 0.5
	_quad = Mesh::CreateQuad(vec4(0, 0, 1, 1), 2, false); 

	_debugMode = 0;

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	testFont = new Font("res/Roboto.ttf");

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;

	std::vector<vec3> ssaoNoise;
	for (i32 i = 0; i < 16; i++) {
		vec3 noise(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	_ssaoNoise = new Texture(vec2i(4, 4), TextureType::Texture2D, TextureFilter::Nearest, TextureAttachment::None, TextureFormat::RGB16, (u8*)&ssaoNoise[0], TextureDataFormat::Float, TextureWrapMode::Repeat);

	_ssaoShader = new Shader("res/ssaoresolve.vert", "res/ssaoresolve.frag");

	_shadowQuality = (ShadowQuality)-1;
	SetShadowQuality((ShadowQuality)Settings::Get<i32>("shadows"));
	SetSSAOSetting(/*(SSAOSetting)Settings::Get<i32>("ssao")*/SSAOSetting::Off);
	
	_shadowCascadeBounds[1] = 10;
	_shadowCascadeBounds[2] = 25;
}

void RenderingEngine::Unload() {
	delete _renderer;

	delete _shadowShader;
	delete _geomShader;

	delete _shadowMap;
}

void RenderingEngine::SetActorReference(std::vector<std::shared_ptr<Core::Actor>>* actors) {
	_actors = actors;
}

void RenderingEngine::SetCamera(Camera* const camera) {
	_camera = camera;
}

void RenderingEngine::Submit(Light* light) {
	_lights.push_back(light);
}

void RenderingEngine::Submit(Mesh* mesh, Material* mat) {
	if (std::find(_meshesToRender.begin(), _meshesToRender.end(), MeshEntry{ mesh, mat }) == _meshesToRender.end()) {
		_meshesToRender.push_back({ mesh, mat });
	}
}

void RenderingEngine::Draw() {
	Timer frameTimer;
	frameTimer.Start();

	u32 verts = 0, faces = 0, passes = 0, calls = 0;

	_lights.clear();

	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);

	for (auto& a : *_actors) {
		a->PreDraw(this, _camera);
	}

	_shadowCascadeBounds[0] = _camera->GetNear();
	_shadowCascadeBounds[3] = _camera->GetFar() / 2;

	//glFinish();
	_preTime += frameTimer.LoopMS();
	
	vec3 cameraLightTrans = _camera->GetTransform().GetTranslation();// +
		//vec3(_camera->GetTransform().GetRotation().Forward().x, 0, _camera->GetTransform().GetRotation().Forward().z) * 10;
	cameraLightTrans.y = 2;

	// Prepare geometry pass.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	Camera* shadowCameras = 0; 
	if (_shadowMap) {
		passes++;
		for (Light* light : _lights) {
			if (light->GetType() == LightType::Directional) {
				shadowCameras = GetShadowMapProjections(cameraLightTrans, (DirectionalLight*)light);
				glCullFace(GL_FRONT);
				_shadowShader->Bind();
				for (i32 i = 0; i < SHADOW_NUM_CASCADES; i++) {
					_shadowMap[i]->BindAsRenderTarget();
					_shadowMap[i]->Clear();

					for (auto& a : *_actors) {
						a->Draw(this, _shadowShader, &shadowCameras[i]);
					}
					FlushMeshes(_shadowShader, false, 0, 0, &calls);
				}
				glCullFace(GL_BACK);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	}

	//glFinish();

	/*if (_blur != 0) {
		_blur->Bind();
		_blur->SetUniform("scale", vec2(1, 0));
		ApplyFilter(_blur, _shadowMap, _shadowMapPong);
		_blur->SetUniform("scale", vec2(0, 1));
		ApplyFilter(_blur, _shadowMapPong, _shadowMap);
		calls += 2;
	}*/

	_shadowTime += frameTimer.LoopMS();

	_gbuffer->BindForWriting();
	_gbuffer->BindAsRenderTarget();
	_renderer->Clear();

	_geomShader->Bind();
	_geomShader->SetUniform("CameraPlanes", vec2(_camera->GetNear(), _camera->GetFar()));
	for (auto& a : *_actors) {
		a->Draw(this, _geomShader, _camera);
	}
	passes++;
	FlushMeshes(_geomShader, true, &verts, &faces, &calls);

	//glFinish();
	_geomTime += frameTimer.LoopMS();

	// SSAO
	glDisable(GL_DEPTH_TEST);
	_gbuffer->BindForReading();
	_gbuffer->Bind(0, 0);
	_gbuffer->Bind(1, 1);

	if (_ssaoSetting == SSAOSetting::On) {
		passes++;
		_ssaoBuffer->BindAsRenderTarget();
		_ssaoShader->Bind();
		_ssaoShader->SetUniform("Noise", _ssaoNoise);
		_ssaoShader->SetUniform("Screen", vec2((f32)Settings::Get<i32>("resolution_x"), (f32)Settings::Get<i32>("resolution_y")));
		_ssaoShader->SetUniform("Projection", _camera->GetViewMatrix());
		_quad->Submit(0, mat4().CreateIdentity(), Camera::Identity);
		_quad->Flush(_ssaoShader);
		calls++;
	}

	// Prepare lighting pass.
	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);
	Renderer::SetBlendState(BlendState::Additive);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (_debugMode == 0) {
		_renderer->Clear();

		_gbuffer->Bind(2, 2);
		_gbuffer->Bind(3, 3);

			// Disable backface culling so we can render the light while inside its radius.
		_pbrShader->Bind();
		_pbrShader->SetUniform("gSSAO", 7);
		_ssaoBuffer->Bind(7);
		_pbrShader->SetUniform("ssaoScale", (f32)_ssaoSetting);
		_pbrShader->SetUniform("screen", vec2((f32)Settings::Get<i32>("resolution_x"), (f32)Settings::Get<i32>("resolution_y")));
		_pbrShader->SetUniform("eyePos", GetCamera()->GetTransform().GetTranslation());
		_pbrShader->SetUniform("gPosition", 0);
		_pbrShader->SetUniform("gNormal", 1);
		_pbrShader->SetUniform("gDiffuse", 2);
		_pbrShader->SetUniform("gSpecRoughness", 3);
		_pbrShader->SetUniform("shadowMS", (i32)_shadowMapSize);

		_pbrShader->SetUniform("PreintegratedFG", 5);
		_pbrShader->SetUniform("EnvironmentMap", 6);
		if (_environmentMap.IsLoaded()) {
			_environmentMap->Bind(6);
		}
		_preFG->Bind(5);

		for (Light* l : _lights) {
			passes++;
			calls++;
			if (l->GetType() == LightType::Point) {
				//glDisable(GL_CULL_FACE);
				//glFrontFace(GL_CCW);
				PointLight* pl = static_cast<PointLight*>(l);
				//Transform t(pl->GetPosition(), PointLightSize(pl));
				//
				//_sphere->Submit(t.GetTransformation(), _camera->GetViewMatrix() * t.GetTransformation());
				_pbrShader->SetUniform("pLight", *pl);
				_pbrShader->SetUniform("light", 1);
				_quad->Submit(0, mat4().CreateIdentity(), Camera::Identity);
				_quad->Flush(_pbrShader);
				//_sphere->Flush(mat.first->GetShader());
				//glFrontFace(GL_CW);
				//glEnable(GL_CULL_FACE);
			}
			else if (l->GetType() == LightType::Directional) {
				DirectionalLight* dl = static_cast<DirectionalLight*>(l);

				if (_shadowMap) {
					for (i32 i = 0; i < SHADOW_NUM_CASCADES; i++) {
						mat4 proj = _camera->GetProjection();
						vec4 v(0, 0, _shadowCascadeBounds[i + 1], 1.0f);
						vec4 vc = proj.Transform(v);

						_pbrShader->SetUniform("shadowBounds[" + std::to_string(i) + "]", vc.z);
						_pbrShader->SetUniform("shadowMap[" + std::to_string(i) + "]", 16 + i);
						_shadowMap[i]->Bind(16 + i);
						_pbrShader->SetUniform("lightMVP[" + std::to_string(i) + "]", shadowCameras[i].GetViewMatrix());
					}

					delete[] shadowCameras;
				}

				_pbrShader->SetUniform("dLight", *dl);
				_pbrShader->SetUniform("light", 0);
					

				_quad->Submit(0, mat4().CreateIdentity(), Camera::Identity);
				_quad->Flush(_pbrShader);
					
			}
			else if (l->GetType() == LightType::Spot) {
				SpotLight* dl = static_cast<SpotLight*>(l);

				_quad->Submit(0, mat4().CreateIdentity(), Camera::Identity);
				_pbrShader->SetUniform("spotLight", *dl);
				_pbrShader->SetUniform("light", 2);
				_quad->Flush(_pbrShader);
			}
		}
		_currentShader = 0;
	}

	_accTime += frameTimer.LoopMS();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	if (_debugMode == 0) {
		_gbuffer->BindForReading();
		glBlitFramebuffer(0, 0, Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y"), 0, 0, 
			Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y"), GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		for (auto& a : *_actors) {
			a->PostDraw(this, _camera);
		}
	}

	_postTime += frameTimer.LoopMS();

	glEnable(GL_BLEND);
	Renderer::SetBlendState(BlendState::Additive);
	glDepthMask(GL_FALSE);

	// Transparent pass / particle pass

	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	Renderer::SetBlendState(BlendState::AlphaBlend);

	if (_debugMode) {
		FillMode start = Renderer::GetFillMode();
		Renderer::SetFillMode(FillMode::Normal);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		if(_debugMode == 1) DrawGuiQuad(Transform(vec3(0.5, 0.5, 0.0), vec3(0.5, 0.5, 1)), _gbuffer, 0);
		if(_debugMode == 2) DrawGuiQuad(Transform(vec3(1.5, 0.5, 0.0), vec3(0.5, 0.5, 1)), _gbuffer, 1);
		if(_debugMode == 3) DrawGuiQuad(Transform(vec3(2.5, 0.5, 0.0), vec3(0.5, 0.5, 1)), _gbuffer, 2);
		if(_debugMode == 4) DrawGuiQuad(Transform(vec3(0.5, 1.5, 0.0), vec3(0.5, 0.5, 1)), _gbuffer, 3);
		
		if (_shadowMap[0]) {
			if (_debugMode == 5) DrawGuiQuad(Transform(vec3(2.5, 1.5, 0.0), vec3(0.5, 0.5, 1)), _shadowMap[0], 0);
			//if (_debugMode == 5) DrawGuiQuad(Transform(vec3(2.5, 1.5, 0.0), vec3(0.5, 0.5, 1)), _shadowMap[1], 0);
			//if(_debugMode == 5) DrawGuiQuad(Transform(vec3(2.5, 1.5, 0.0), vec3(0.5, 0.5, 1)), _shadowMap[2], 0);
		}
		
		if (_debugMode == 6) DrawGuiQuad(Transform(vec3(2.5, 1.5, 0.0), vec3(0.5, 0.5, 1)), _ssaoBuffer, 0);
		Renderer::SetFillMode(start);
	}

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	Camera guiCamera = Camera(mat4().CreateOrthographic(0, (f32)Renderer::UI_RESOLUTION_X, (f32)Renderer::UI_RESOLUTION_Y, 0, -50.f, 50.f));
	_guiShader->Bind();

	for (auto& a : *_actors) {
		a->PreDraw2D(this, &guiCamera);
	}

	for (auto& a : *_actors) {
		a->Draw2D(this, _guiShader, &guiCamera);
	}

	for (auto& a : *_actors) {
		a->PostDraw2D(this, &guiCamera);
	}

	_2dTime += frameTimer.LoopMS();
	 
	f32 cputime = _timer.LoopMS();
	_time += cputime;
	_framesRendered++;

	_cpuRenderTime = cputime;
	
	if (_time >= 1000) {
		_preTimeFrame = _preTime / _framesRendered;
		_shadowTimeFrame = _shadowTime / _framesRendered;
		_geomTimeFrame = _geomTime / _framesRendered;
		_accTimeFrame = _accTime / _framesRendered;
		_postTimeFrame = _postTime / _framesRendered;
		_2dTimeFrame = _2dTime / _framesRendered;
		_frameTime = _time / _framesRendered;


		_preTime = 0;
		_shadowTime = 0;
		_geomTime = 0;
		_accTime = 0;
		_postTime = 0;
		_2dTime = 0;
/*
		Logger::Info(
R"(
-------- FRAME STATS --------
Pre pass:          %.3fms
Shadow pass:       %.3fms
Geometry pass:     %.3fms
Accumulation pass: %.3fms
Post pass:         %.3fms
2D pass:           %.3fms
CPU time:          %.3fms
Frame time:        %.3fms
FPS:               %d
-----------------------------)", _preTimeFrame, _shadowTimeFrame, _geomTimeFrame, _accTimeFrame, _postTimeFrame, 
								 _2dTimeFrame, _cpuRenderTime, _frameTime, _framesRendered);*/

		_framesRendered = 0;
		_time = 0;
	}

	_vertsPerFrame = verts;
	_facesPerFrame = faces;
	_passesPerFrame = passes;
	_drawcallsPerFrame = calls;
}

void RenderingEngine::FlushMeshes(Shader* shader, bool materialize, u32* verts, u32* faces, u32* calls) {
	Material* current = 0;
	if (materialize) std::sort(_meshesToRender.begin(), _meshesToRender.end());

	for (auto& a : _meshesToRender) {
		if (materialize) {
			if (current != a.material) {
				current = a.material;
				current->Bind(shader);
			}
		}
		if (verts) *verts += a.mesh->GetNumVerts() * a.mesh->GetQueued();
		if (faces) *faces += a.mesh->GetNumFaces() * a.mesh->GetQueued();
		if (calls) *calls += a.mesh->GetQueued();
		a.mesh->Flush(shader);
	}

	_meshesToRender.clear();
}

f32 RenderingEngine::PointLightSize(PointLight* pl) {
	f32 MaxChannel = fmax(fmax(pl->GetColor().x, pl->GetColor().y), pl->GetColor().z);

	f32 ret = (-pl->GetAttenuation().GetLinear() + sqrtf(pl->GetAttenuation().GetLinear() * pl->GetAttenuation().GetLinear() -
		4.f * pl->GetAttenuation().GetExponent() * (pl->GetAttenuation().GetExponent() - 256.f * MaxChannel * pl->GetDiffuseIntensity()))) /
		2.f * pl->GetAttenuation().GetExponent() / 2;
	return ret * 10;
}

void RenderingEngine::DrawGuiQuad(Transform t, Texture* tex, u32 subid) {
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_guiShader->Bind();
	_guiShader->SetUniform("diffuse", 0);
	tex->Bind(0, subid);
	_quad->Submit(0, mat4().CreateIdentity(), Camera::Identity);
	_quad->Flush(_guiShader);
	_guiShader->Unbind();
}

i32 RenderingEngine::PushMaterial(Material* material) {
	//for (auto& mat : _materialsToRender) {
	//	if (mat.first->GetName() == material->GetName()) {
	//		return mat.second;
	//	}
	//}

	//_materialsToRender.push_back(std::make_pair(material, _maxMaterial));
	//_maxMaterial++;
	//return _maxMaterial - 1;
	return 0;
}

void RenderingEngine::SetShadowQuality(ShadowQuality quality) {
	glEnable(GL_MULTISAMPLE);
	if (quality != _shadowQuality) {
		_shadowQuality = quality;
		switch (quality) {
		case ShadowQuality::Off:
			_shadowMapSize = 0;
			_blur = 0;
			break;

		case ShadowQuality::Terrible:
			_shadowMapSize = 512;
			_blur = 0;
			GenerateShadowMaps();
			break;

		case ShadowQuality::Low:
			_shadowMapSize = 1024;
			_blur = 0;
			GenerateShadowMaps();
			break;

		case ShadowQuality::Medium:
			_shadowMapSize = 2048;
			_blur = _blur7;
			GenerateShadowMaps();
			break;

		case ShadowQuality::High:
			_shadowMapSize = 2048;
			_blur = _blur7;
			GenerateShadowMaps();
			break;

		case ShadowQuality::Ultra:
			_shadowMapSize = 4096;
			_blur = _blur9;
			GenerateShadowMaps();
			break;
		}
	}
}

void RenderingEngine::GenerateShadowMaps() {
	for (i32 i = 0; i < SHADOW_NUM_CASCADES; i++) {
		if (_shadowMap[i]) delete _shadowMap[i];

		_shadowMap[i] = new Texture(vec2i(_shadowMapSize), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::R32, 0, TextureDataFormat::Float);
	}
}

Renderer* RenderingEngine::GetRenderer() {
	return _renderer;
}

void RenderingEngine::ApplyFilter(Shader* filter, Texture* src, Texture* dest) const {
	filter->Bind();
	filter->SetUniform("diffuse", src);
	dest->BindAsRenderTarget();
	_quad->Submit(0, mat4().CreateIdentity(), Camera::Identity);
	_quad->Flush(filter);
}

Camera* RenderingEngine::GetShadowMapProjections(vec3 position, DirectionalLight* light) {
	Camera* ret = new Camera[SHADOW_NUM_CASCADES];
	mat4 cam = _camera->GetTransform().GetCameraTransformation();
	mat4 camInv = cam.Inverse();
	
	Transform t(vec3(0), vec3(1), light->GetDirection());
	mat4 lightM = t.GetCameraTransformation();
	f32 fov = _camera->GetFOV();// *1.5f;

	f32 thhf = tanf(fov / 2);
	f32 thvf = tanf((fov * _camera->GetAspect()) / 2);

	for (u32 i = 0; i < SHADOW_NUM_CASCADES; i++) {
		f32 xn = _shadowCascadeBounds[i] * thhf;
		f32 xf = _shadowCascadeBounds[i + 1] * thhf;
		f32 yn = _shadowCascadeBounds[i] * thvf;
		f32 yf = _shadowCascadeBounds[i + 1] * thvf;

		vec4 corners[8] = {
			vec4(xn, yn, _shadowCascadeBounds[i], 1.0),
			vec4(-xn, yn, _shadowCascadeBounds[i], 1.0),
			vec4(xn, -yn, _shadowCascadeBounds[i], 1.0),
			vec4(-xn, -yn, _shadowCascadeBounds[i], 1.0),

			vec4(xf, yf, _shadowCascadeBounds[i + 1], 1.0),
			vec4(-xf, yf, _shadowCascadeBounds[i + 1], 1.0),
			vec4(xf, -yf, _shadowCascadeBounds[i + 1], 1.0),
			vec4(-xf, -yf, _shadowCascadeBounds[i + 1], 1.0),
		};

		f32 minX = 100'000'000;
		f32 maxX = -100'000'000;
		f32 minY = 100'000'000;
		f32 maxY = -100'000'000;
		f32 minZ = 100'000'000;
		f32 maxZ = -100'000'000;

		vec4 cornersL[8];
		for (u32 j = 0; j < 8; j++) {
			vec4 vw = camInv.Transform(corners[j]);
			cornersL[j] = lightM.Transform(vw);

			minX = fminf(minX, cornersL[j].x);
			maxX = fmaxf(maxX, cornersL[j].x);

			minY = fminf(minY, cornersL[j].y);
			maxY = fmaxf(maxY, cornersL[j].y);

			minZ = fminf(minZ, cornersL[j].z);
			maxZ = fmaxf(maxZ, cornersL[j].z);
		}

		ret[i] = Camera(mat4().CreateOrthographic(minX - 5, maxX + 5, minY - 5, maxY + 5, minZ - 30, maxZ + 30), Transform(vec3(0), vec3(1), light->GetDirection()));
	}

	return ret;
}

void RenderingEngine::SetSSAOSetting(SSAOSetting setting) {
	_ssaoSetting = setting;
}

void RenderingEngine::Update() {
	if (Keyboard::IsKeyPressed(Key::Four) && !_debugUI) {
		_debugUI = std::make_shared<Actor>();

		std::shared_ptr<Actor> bar1 = std::make_shared<Actor>(Transform(vec3(110, 120, 0), vec3(200, 30, 1)));
		std::shared_ptr<Actor> bar2 = std::make_shared<Actor>(Transform(vec3(110, 170, 0), vec3(200, 30, 1)));
		
		bar1->AddComponent(std::make_shared<UITweakBar>(_camera->GetNear(), _camera->GetFar(), testFont));
		bar2->AddComponent(std::make_shared<UITweakBar>(_camera->GetNear(), _camera->GetFar(), testFont));

		_debugUI->AddChild(bar1);
		_debugUI->AddChild(bar2);
		_actors->push_back(_debugUI);
	} else if (Keyboard::IsKeyPressed(Key::Four) && _debugUI) {
		_debugUI->Kill();
		//delete _debugUI;
		_debugUI = 0;
	}

	if (_debugUI) {
		_shadowCascadeBounds[1] = _debugUI->GetChildren()[0]->GetComponent<UITweakBar>()->GetValue();
		_shadowCascadeBounds[2] = _debugUI->GetChildren()[1]->GetComponent<UITweakBar>()->GetValue();
	}
}