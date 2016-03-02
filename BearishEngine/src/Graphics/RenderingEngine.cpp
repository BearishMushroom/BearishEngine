#include "RenderingEngine.h"

#include "../Core/Transform.h"
#include "../Core/Window.h"
#include "../Core/Assets.h"
#include "../Core/Actor.h"
#include "../Core/Model.h"

#include "Renderer.h"
#include "Texture.h"
#include "Camera.h"
#include "Lights.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh/Mesh.h"

#include "../Utils.h"

using namespace Bearish;
using namespace Core;
using namespace Graphics;
using namespace Math;

f32 timef = 0;

RenderingEngine::RenderingEngine() {

}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::Load() {
	OpenGL::Init();
	_renderer = new Renderer();

	_phongShader = new Shader("res/phong.vert", "res/phong.frag");
	_shadowShader = new Shader("res/shadowmap.vert", "res/shadowmap.frag");
	_guiShader = new Shader("res/gui.vert", "res/gui.frag");

	SetShadowQuality(ShadowQuality::High);

	std::vector<TextureAttachment> att = {
		TextureAttachment::Color0,
		TextureAttachment::Color1,
		TextureAttachment::Color2,
		TextureAttachment::Color3,
	};

	std::vector<TextureFormat> fmt {
		TextureFormat::RGB32,
		TextureFormat::RGB32,
		TextureFormat::RGB32,
		TextureFormat::RGB32,
	};

	_gbuffer = new Texture(vec2(1280, 720), TextureType::Texture2D, att, fmt, 4);

	_geomShader = new Shader("res/geometrypass.vert", "res/geometrypass.frag");
	_geomShader->SetName("geom");
	_sphere = new Mesh(Model(Assets::Get("sphere")).ToMesh());

	_quad = new Mesh(std::vector<Vertex> {
		Vertex(vec3(-1, -1, 0), vec2(0, 0)),
			Vertex(vec3(-1, 1, 0), vec2(0, 1)),
			Vertex(vec3(1, 1, 0), vec2(1, 1)),
			Vertex(vec3(1, -1, 0), vec2(1, 0)),
	}, std::vector<u32> {0, 2, 3, 0, 1, 2});

	_debugMode = 0;

	testFont = new Font("res/Roboto.ttf");

	testPart = new ParticleSystem(new Texture(Assets::Get("particle")),
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
}

void RenderingEngine::Unload() {
	delete _renderer;

	delete _phongShader;
	delete _shadowShader;
	delete _geomShader;

	delete _shadowMap;
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
	_materialsToRender.clear();
	_maxMaterial = 0;
	_lights.clear();

	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);

	for (auto& a : *_actors) {
		a->PreDraw(this, _camera);
	}

	// Prepare geometry pass.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	if (_shadowMap) {
		for (Light* light : _lights) {
			if (light->GetType() == LightType::Directional) {
				_shadowMap->BindAsRenderTarget();
				_shadowMap->Clear();
				_shadowShader->Bind();
				Camera scamera = Camera(mat4().CreateOrthographic(-15, 15, -15, 15, -40, 40), Transform(vec3(0, 5, -5),
					vec3(1), ((DirectionalLight*)light)->GetDirection()));

				glCullFace(GL_FRONT);

				for (auto& a : *_actors) {
					a->Draw(this, _shadowShader, &scamera);
				}
				FlushMeshes(false);

				glCullFace(GL_BACK);

				_window->BindAsRenderTarget();
			}
		}
	}

	_gbuffer->BindForWriting();
	_gbuffer->BindAsRenderTarget();
	_renderer->Clear();

	_geomShader->Bind();

	for (auto& a : *_actors) {
		a->Draw(this, _geomShader, _camera);
	}
	FlushMeshes(true);

	// Prepare lighting pass.
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	Renderer::SetBlendState(BlendState::Additive);

	_window->BindAsRenderTarget();
	_gbuffer->BindForReading();
	if (_debugMode == 0) {
		_renderer->Clear();

		_gbuffer->Bind(0, 0);
		_gbuffer->Bind(1, 1);
		_gbuffer->Bind(2, 2);
		_gbuffer->Bind(3, 3);

			// Disable backface culling so we can render the light while inside its radius.
		for (Light* l : _lights) {
			for (auto& mat : _materialsToRender) {
				bool shaderChanged = false;

				if (_currentShader != mat.first->GetShader()) {
					mat.first->GetShader()->Bind();
					mat.first->GetShader()->SetUniform("eyePos", GetCamera()->GetTransform().GetTranslation());
					mat.first->GetShader()->SetUniform("gWorld", 0);
					mat.first->GetShader()->SetUniform("gTangent", 1);
					mat.first->GetShader()->SetUniform("gNormal", 2);
					mat.first->GetShader()->SetUniform("gTexCoord", 3);
					mat.first->GetShader()->SetUniform("pcfSize", (i32)_shadowPcfSize);
					mat.first->GetShader()->SetUniform("shadowSize", _shadowMap == nullptr ? 0 : (i32)_shadowMap->GetSize().x);
					_currentShader = mat.first->GetShader();
					shaderChanged = true;
				}
				mat.first->Bind(this);
				mat.first->GetShader()->SetUniform("matID", mat.second);

				if (l->GetType() == LightType::Point) {
					//glDisable(GL_CULL_FACE);
					glFrontFace(GL_CCW);
					PointLight* pl = static_cast<PointLight*>(l);
					Transform t(pl->GetPosition(), PointLightSize(pl));
					_sphere->Submit(t.GetTransformation(), _camera->GetViewMatrix() * t.GetTransformation());
					if (shaderChanged) {
						mat.first->GetShader()->SetUniform("pLight", *pl);
						mat.first->GetShader()->SetUniform("light", 1);
					}
					_sphere->Flush();
					glFrontFace(GL_CW);
					//glEnable(GL_CULL_FACE);
				}
				else if (l->GetType() == LightType::Directional) {
					DirectionalLight* dl = static_cast<DirectionalLight*>(l);
					Camera scamera = Camera(mat4().CreateOrthographic(-15, 15, -15, 15, -30, 30), Transform(vec3(0, 5, -5),
						vec3(1), dl->GetDirection()));

					if (shaderChanged) {
						if (_shadowMap) {
							mat.first->GetShader()->SetUniform("shadowMap", 4);
							_shadowMap->Bind(4);
						}

						mat.first->GetShader()->SetUniform("lightMVP", scamera.GetViewMatrix());
						mat.first->GetShader()->SetUniform("dLight", *dl);
						mat.first->GetShader()->SetUniform("light", 0);
					}

					_quad->Submit(mat4().CreateIdentity(), mat4().CreateIdentity());
					_quad->Flush();
					
				}
				else if (l->GetType() == LightType::Spot) {
					SpotLight* dl = static_cast<SpotLight*>(l);

					_quad->Submit(mat4().CreateIdentity(), mat4().CreateIdentity());
					if (shaderChanged) {
						mat.first->GetShader()->SetUniform("spotLight", *dl);
						mat.first->GetShader()->SetUniform("light", 2);
					}
					_quad->Flush();
				}
			}
			_currentShader = 0;
		}
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	if (_debugMode == 0) {
		_gbuffer->BindForReading();
		glBlitFramebuffer(0, 0, 1280, 1280, 0, 0, 1280, 1280, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		for (auto& a : *_actors) {
			a->PostDraw(this, _camera);
		}
	}

	glEnable(GL_BLEND);
	Renderer::SetBlendState(BlendState::Additive);
	glDepthMask(GL_FALSE);

	testPart->Draw(_camera);

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

}

void RenderingEngine::FlushMeshes(bool bind) {
	for (auto& a : _meshesToRender) {
		if (bind) {
			_geomShader->SetUniform("matID", _maxMaterial);
			PushMaterial(a.material);
		}
		a.mesh->Flush();
	}
	_meshesToRender.clear();
}

f32 RenderingEngine::PointLightSize(PointLight* pl) {
	f32 MaxChannel = fmax(fmax(pl->GetColor().x, pl->GetColor().y), pl->GetColor().z);

	f32 ret = (-pl->GetAttenuation().GetLinear() + sqrtf(pl->GetAttenuation().GetLinear() * pl->GetAttenuation().GetLinear() -
		4.f * pl->GetAttenuation().GetExponent() * (pl->GetAttenuation().GetExponent() - 256.f * MaxChannel * pl->GetDiffuseIntensity()))) /
		2.f * pl->GetAttenuation().GetExponent() / 24.f;
	return ret;
}

void RenderingEngine::DrawGuiQuad(Transform t, Texture* tex, u32 subid) {
	glDisable(GL_DEPTH_TEST);
	_window->BindAsRenderTarget();
	_guiShader->Bind();
	_guiShader->SetUniform("diffuse", 0);
	tex->Bind(0, subid);
	_quad->Submit(mat4().CreateIdentity(), mat4().CreateIdentity());
	_quad->Flush();
	_guiShader->Unbind();
}

i32 RenderingEngine::PushMaterial(Material* material) {
	for (auto& mat : _materialsToRender) {
		if (mat.first->GetName() == material->GetName()) {
			return mat.second;
		}
	}

	_materialsToRender.push_back(std::make_pair(material, _maxMaterial));
	_maxMaterial++;
	return _maxMaterial - 1;
}

void RenderingEngine::SetShadowQuality(ShadowQuality quality) {
	if (quality != _shadowQuality) {
		_shadowQuality = quality;
		delete _shadowMap;
		_shadowMap = nullptr;
		switch (quality) {
		case ShadowQuality::Off:
			_shadowPcfSize = 0;
			break;

		case ShadowQuality::Terrible:
			_shadowMap = new Texture(vec2(512), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::R32);
			_shadowPcfSize = 0;
			break;

		case ShadowQuality::Low:
			_shadowMap = new Texture(vec2(1024), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::R32);
			_shadowPcfSize = 1;
			break;

		case ShadowQuality::Medium:
			_shadowMap = new Texture(vec2(2048), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::R32);
			_shadowPcfSize = 2;
			break;

		case ShadowQuality::High:
			_shadowMap = new Texture(vec2(2048), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::R32);
			_shadowPcfSize = 3;
			break;

		case ShadowQuality::Ultra:
			_shadowMap = new Texture(vec2(4096), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::Color0, TextureFormat::R32);
			_shadowPcfSize = 4;
			break;
		}
	}
}