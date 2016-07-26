#include "RenderingEngine.h"

#include "../Core/Transform.h"
#include "../Core/Window.h"
#include "../Core/Asset.h"
#include "../Core/Actor.h"
#include "../Core/Model.h"
#include "../Core/Timer.h"
#include "../Core/Settings.h"

#include "Renderer.h"
#include "Texture/Texture.h"
#include "Camera.h"
#include "Lights.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh/Mesh.h"

#include "../Utils.h"

#include <random>

using namespace Bearish;
using namespace Core;
using namespace Graphics;
using namespace Math;

f32 timef = 0;

RenderingEngine::RenderingEngine() {
	_timer.Start();
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::Load() {
	OpenGL::Init();
	_renderer = new Renderer();

	_phongShader = new Shader("res/phong.vert", "res/phong.frag");
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

	// Old Gbuffer size at 720p:
	/*
		Position buffer : 1280 * 720 * (RGB32)12 = 10.5MB
		UV buffer       : 1280 * 720 * (RGB32)12 = 10.5MB
		Normal buffer   : 1280 * 720 * (RGB32)12 = 10.5MB
		Tangent buffer  : 1280 * 720 * (RGB32)12 = 10.5MB
		Total           : 10.5 * 4 = 42MB of VRAM
		Total BPP       : 48 * 8 = 384 BPP
	*/

	// New Gbuffer at 720p:
	/*
		Position buffer     : 1280 * 720 * (RGBA32)16 = 14.1MB
		Normal buffer       : 1280 * 720 * (RG16)   4 = 3.5MB
		Diffuse buffer      : 1280 * 720 * (RGB8)   3 = 2.6MB
		Spec + gloss buffer : 1280 * 720 * (RGBA8)  4 = 3.5MB
		Total               : 14.1 + 3.5 + 2.6 + 3.5 = 23.7MB of VRAM
		Total BPP           : 27 * 8 = 216 BPP
	*/

	std::vector<TextureFormat> fmt {
		TextureFormat::RGBA32, // Position
		TextureFormat::RGB16,  // Normal XY
		TextureFormat::RGB,   // Diffuse
		TextureFormat::RGBA,  // Spec + gloss
	};


	_gbuffer = new Texture(vec2(Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y")), TextureType::Texture2D, att, fmt, 4);
	_ssaoBuffer = new Texture(vec2(Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y")), TextureType::Texture2D, TextureFilter::Nearest, TextureAttachment::Color0, TextureFormat::R, 0);

	_geomShader = new Shader("res/geometrypass.vert", "res/geometrypass.frag");
	_geomShader->SetName("geom");
	_pbrShader = new Shader("res/PBR.vert", "res/PBR.frag");
	_sphere = new Mesh(Model("asset/sphere.bem").ToMesh());

	_quad = new Mesh(std::vector<Vertex> {
		Vertex(vec3(-1, -1, 0), vec2(0, 0)),
			Vertex(vec3(-1, 1, 0), vec2(0, 1)),
			Vertex(vec3(1, 1, 0), vec2(1, 1)),
			Vertex(vec3(1, -1, 0), vec2(1, 0)),
	}, std::vector<u32> {0, 2, 3, 0, 1, 2});

	_debugMode = 0;

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	testFont = new Font("res/Roboto.ttf");

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;

	std::vector<vec3> ssaoNoise;
	for (i32 i = 0; i < 16; i++) {
		vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	_ssaoNoise = new Texture(vec2(4, 4), TextureType::Texture2D, TextureFilter::Nearest, TextureAttachment::None, TextureFormat::RGB16, (u8*)&ssaoNoise[0], TextureDataFormat::Float, TextureWrapMode::Repeat);

	_ssaoShader = new Shader("res/ssaoresolve.vert", "res/ssaoresolve.frag");

	testPart = new ParticleSystem(new Texture("asset/particle.bet"),
	[](const vec3& pos) -> Particle {
		timef += 1.f / 60.f / 16.f;
		Particle ret;
		ret.position = pos + vec3((Util::Random() - 0.5f) * 12.f, 0, (Util::Random() - 0.5f) * 1.5f);
		ret.velocity = vec3(Util::Random() / 6.f, Util::Random(), Util::Random() / 6.f);// +vec3((Util::Random() - 0.5f) * 2.f, Util::Random(), (Util::Random() - 0.5f) * 2.f);
		ret.life = 4.5f;
		ret.scale = vec3(1);
		ret.color = vec4(sinf(timef), cosf(timef * 2), tanf(timef * 3), 3);
		return ret;
	}, [](Particle* p, f32 dt) -> void {
		p->position += p->velocity * dt;
		//p->velocity += vec3(0, 1, 0) * dt;
		p->color += vec4(0, 0.0035f, 0, -.75f / 60.f);
		p->scale += vec3(0.01f);
		p->life -= dt;
	});

	_shadowQuality = (ShadowQuality)-1;
	SetShadowQuality((ShadowQuality)Settings::Get<i32>("shadows"));
	SetSSAOSetting(/*(SSAOSetting)Settings::Get<i32>("ssao")*/SSAOSetting::Off);
}

void RenderingEngine::Unload() {
	delete _renderer;

	delete _phongShader;
	delete _shadowShader;
	delete _geomShader;

	delete _shadowMap;
	delete _shadowMapPong;
}

void RenderingEngine::SetActorReference(std::vector<Core::Actor*>* actors) {
	_actors = actors;
}

void RenderingEngine::SetCamera(Camera* const camera) {
	_camera = camera;
}

void RenderingEngine::SetWindow(Window* const window) {
	_window = window;
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

	//glFinish();
	_preTime += frameTimer.LoopMS();
	
	vec3 cameraLightTrans = _camera->GetTransform().GetTranslation();// +
		//vec3(_camera->GetTransform().GetRotation().Forward().x, 0, _camera->GetTransform().GetRotation().Forward().z) * 10;
	cameraLightTrans.y = 2;

	// Prepare geometry pass.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	if (_shadowMap) {
		passes++;
		for (Light* light : _lights) {
			if (light->GetType() == LightType::Directional) {
				_shadowMap->BindAsRenderTarget();
				_shadowMap->Clear();
				_shadowShader->Bind();

				Camera scamera = GetShadowMapProjection(cameraLightTrans, (DirectionalLight*)light);

				glCullFace(GL_FRONT);

				for (auto& a : *_actors) {
					a->Draw(this, _shadowShader, &scamera);
				}
				FlushMeshes(_shadowShader, false, 0, 0, &calls);

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
	_geomShader->SetUniform("CameraPlanes", vec2(0.1f, 100.f));
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
		_ssaoShader->SetUniform("Screen", vec2(Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y")));
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
		_pbrShader->SetUniform("screen", vec2(Settings::Get<i32>("resolution_x"), Settings::Get<i32>("resolution_y")));
		_pbrShader->SetUniform("eyePos", GetCamera()->GetTransform().GetTranslation());
		_pbrShader->SetUniform("gPosition", 0);
		_pbrShader->SetUniform("gNormal", 1);
		_pbrShader->SetUniform("gDiffuse", 2);
		_pbrShader->SetUniform("gSpecRoughness", 3);
		_pbrShader->SetUniform("shadowMS", (i32)_shadowMapSize);

		_pbrShader->SetUniform("PreintegratedFG", 5);
		_pbrShader->SetUniform("EnvironmentMap", 6);
		_environmentMap->Bind(6);
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

				Camera scamera = GetShadowMapProjection(cameraLightTrans, dl);

				if (_shadowMap) {
					_pbrShader->SetUniform("shadowMap", 4);
					_shadowMap->Bind(4);
				}

				_pbrShader->SetUniform("lightMVP", scamera.GetViewMatrix());
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

	//glFinish();
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

	//glFinish();
	_postTime += frameTimer.LoopMS();


	glEnable(GL_BLEND);
	Renderer::SetBlendState(BlendState::Additive);
	glDepthMask(GL_FALSE);

	//testPart->Draw(_camera);

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
		if (_shadowMap) {
			if(_debugMode == 5) DrawGuiQuad(Transform(vec3(2.5, 1.5, 0.0), vec3(0.5, 0.5, 1)), _shadowMap, 0);
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

	//glFinish();
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
								 _2dTimeFrame, _cpuRenderTime, _frameTime, _framesRendered);

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
		if(_shadowMap) delete _shadowMap;
		_shadowMap = 0;
		if (_shadowMapPong) delete _shadowMapPong;
		_shadowMapPong = 0;
		switch (quality) {
		case ShadowQuality::Off:
			_shadowMapSize = 0;
			_blur = 0;
			_shadowSamples = 0;
			break;

		case ShadowQuality::Terrible:
			_shadowMap = new Texture(vec2(512), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapPong = new Texture(vec2(512), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapSize = 512;
			_blur = 0;
			break;

		case ShadowQuality::Low:
			_shadowMap = new Texture(vec2(1024), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapPong = new Texture(vec2(1024), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapSize = 1024;
			_blur = 0;
			break;

		case ShadowQuality::Medium:
			_shadowMap = new Texture(vec2(2048), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapPong = new Texture(vec2(2048), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapSize = 2048;
			_blur = _blur7;
			break;

		case ShadowQuality::High:
			_shadowMap = new Texture(vec2(2048), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapPong = new Texture(vec2(2048), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapSize = 2048;
			_blur = _blur7;
			break;

		case ShadowQuality::Ultra:
			_shadowMap = new Texture(vec2(4096), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapPong = new Texture(vec2(4096), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::RG32, 0, TextureDataFormat::Float);
			_shadowMapSize = 4096;
			_blur = _blur9;
			break;
		}
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

Camera RenderingEngine::GetShadowMapProjection(vec3 position, DirectionalLight* light) {
	/*std::vector<vec3> cube(NDC);
	Transform lightTrans = Transform(cameraLightTrans, vec3(1), dl->GetDirection());
	mat4 invVP = _camera->GetViewMatrix().Inverse();
	mat4 invWorld = lightTrans.GetTransformation().Inverse();

	mat4 trans = invVP * invWorld;

	for (i32 i = 0; i < cube.size(); i++) {
	cube[i] = trans.Transform(cube[i]);
	}

	vec2 x(INFINITY, -INFINITY), y(INFINITY, -INFINITY), z(INFINITY, -INFINITY);

	for (auto& c : cube) {
	x.x = min(x.x, c.x);
	x.y = max(x.y, c.x);

	y.x = min(y.x, c.y);
	y.y = max(y.y, c.y);

	z.x = min(z.x, c.z);
	z.y = max(z.y, c.z);
	}

	f32 xs = abs((x.y - x.x) / 2) + 5;
	f32 ys = abs((y.y - y.x) / 2) + 5;
	f32 zs = abs((z.y - z.x) / 2) + 5;*/

	auto cam = Camera(mat4().CreateOrthographic(-25, 25, -25, 25, -10, 20), Transform(position,
		vec3(1), light->GetDirection()));

	/*mat4 view = cam.GetViewMatrix();
	vec3 origin(0, 0, 0);

	origin = view.Transform(origin);
	f32 texX = origin.x * _shadowMapSize * 0.5f;
	f32 texY = origin.z * _shadowMapSize * 0.5f;

	f32 rX = roundf(texX);
	f32 rY = roundf(texY);

	f32 dx = rX - texX;
	f32 dy = rY - texY;

	dx /= _shadowMapSize * 0.5f;
	dy /= _shadowMapSize * 0.5f;*/

	/*f32 factor = 60 / _shadowMapSize;
	i32 x = roundf(cam.GetTransform().GetTranslation().x / factor) * factor;
	i32 z = roundf(cam.GetTransform().GetTranslation().z / factor) * factor;

	printf("%s\n", cam.GetTransform().GetTranslation().ToString().c_str());
	cam.GetTransform().SetTranslation(cam.GetTransform().GetTranslation() + vec3(-x, 0, -z));
	printf("%s\n---\n", cam.GetTransform().GetTranslation().ToString().c_str());
	*/
	return cam;
}

void RenderingEngine::SetSSAOSetting(SSAOSetting setting) {
	_ssaoSetting = setting;
}
