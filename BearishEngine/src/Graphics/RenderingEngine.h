#ifndef _BEARISH_GRAPHICS_RENDERINGENGINE_H_
#define _BEARISH_GRAPHICS_RENDERINGENGINE_H_

#include "../Core/IAllocatable.h"
#include "../Core/Timer.h"
#include "../Types.h"
#include "Mesh/Mesh.h"
#include "ParticleSystem.h"
#include "Font.h"

namespace Bearish { 
	namespace Core {
		class Actor;
		class Window;
		class Transform;
	}

	namespace Graphics {
	class Shader;
	class Camera;
	class Light;
	class Renderer;
	class Texture;
	class PointLight;
	class Material;

	enum class ShadowQuality : u8 {
		Off,
		Terrible,
		Low,
		Medium,
		High,
		Ultra,
	};

	struct MeshEntry {
		Mesh* mesh;
		Material* material;

		bool operator==(const MeshEntry& o) {
			return o.mesh->GetVAOID() == mesh->GetVAOID() && material == o.material;
		}

		bool operator<(const MeshEntry& o) {
			return material < o.material;
		}
	};

	class RenderingEngine : public Core::IAllocatable<RenderingEngine> {
	public:
		RenderingEngine();
		~RenderingEngine();

		void Load();
		void Unload();

		void SetActorReference(std::vector<Core::Actor*>* actors);
		void SetCamera(Camera* const camera);
		void SetWindow(Core::Window* const window);

		void Submit(Light* light);
		void Submit(Mesh* mesh, Material* mat);

		void Draw();
		void FlushMeshes(Shader* shader, bool bind);

		void DrawGuiQuad(Core::Transform t, Texture* tex, u32 subid = 0);

		std::vector<Core::Actor*>* GetActors() { return _actors; }
		Camera* GetCamera() { return _camera; }
		i32 PushMaterial(Material* material);

		void SetShadowQuality(ShadowQuality quality);

		void SetDebugMode(i32 mode) { _debugMode = mode; }

		void SetEnvironmentMap(Texture* tex) { _environmentMap = tex; }
		void SetPreFG(Texture* tex) { _preFG = tex; }
	private:
		std::vector<Light*> _lights;
		Texture* _gbuffer;
		Renderer* _renderer;

		Mesh* _sphere,* _quad;
		Shader* _currentShader;

		Core::Window* _window;
		Camera* _camera;
		std::vector<Core::Actor*>* _actors;
		i32 _debugMode;

		std::vector<MeshEntry> _meshesToRender;

		Texture* _environmentMap;
		Texture* _preFG;

		Texture* _shadowMap;
		f32 _shadowPcfSize;
		ShadowQuality _shadowQuality;

		f32 PointLightSize(PointLight* pl);

		Font* testFont;

		Core::Timer _timer;

		i32 _framesRendered;
		f32 _time, _shadowTime, _geomTime, _accTime, _preTime, _postTime, _2dTime, _frameTime,
			_shadowTimeFrame, _geomTimeFrame, _accTimeFrame, _preTimeFrame, _postTimeFrame, _2dTimeFrame;
	public:
		Shader* _phongShader, *_shadowShader, *_geomShader, *_guiShader, *_pbrShader;
		ParticleSystem* testPart;
	};
} }

#endif // _BEARISH_GRAPHICS_RENDERINGENGINE_H_