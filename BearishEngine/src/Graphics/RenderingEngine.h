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
		Off = 0,
		Terrible,
		Low,
		Medium,
		High,
		Ultra,
	};

	enum class SSAOSetting : u8 {
		Off = 0, On
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
		void FlushMeshes(Shader* shader, bool bind, u32* verts = 0, u32* faces = 0, u32* calls = 0);

		void DrawGuiQuad(Core::Transform t, Texture* tex, u32 subid = 0);

		std::vector<Core::Actor*>* GetActors() { return _actors; }
		Camera* GetCamera() { return _camera; }
		i32 PushMaterial(Material* material);

		void SetShadowQuality(ShadowQuality quality);
		void SetSSAOSetting(SSAOSetting setting);

		void SetDebugMode(i32 mode) { _debugMode = mode; }

		void SetEnvironmentMap(Texture* tex) { _environmentMap = tex; }
		void SetPreFG(Texture* tex) { _preFG = tex; }

		f32 GetCPUTime() const { return _cpuRenderTime; }

		void ApplyFilter(Shader* filter, Texture* src, Texture* dest) const;
		Camera GetShadowMapProjection(Math::vec3 position, DirectionalLight* light);

		Renderer* GetRenderer();

		u32 GetVertsPerFrame() const { return _vertsPerFrame; }
		u32 GetFacesPerFrame() const { return _facesPerFrame; }
		u32 GetPassesPerFrame() const { return _passesPerFrame; }
		u32 GetDrawcallsPerFrame() const { return _drawcallsPerFrame; }
	private:
		std::vector<Light*> _lights;
		Texture* _gbuffer;
		Renderer* _renderer;

		Mesh* _sphere,* _quad;
		Shader* _currentShader;

		Camera* _camera;
		std::vector<Core::Actor*>* _actors;
		i32 _debugMode;

		std::vector<MeshEntry> _meshesToRender;

		Texture* _environmentMap;
		Texture* _preFG;

		Texture* _shadowMap,* _shadowMapPong;
		f32 _shadowMapSize;
		f32 _shadowSamples;
		ShadowQuality _shadowQuality;

		Texture* _ssaoNoise,* _ssaoBuffer;
		Shader* _ssaoShader;
		SSAOSetting _ssaoSetting;

		f32 PointLightSize(PointLight* pl);

		Shader* _blur,* _blur7,* _blur9;

		Font* testFont;

		Core::Timer _timer;

		u32 _vertsPerFrame, _facesPerFrame, _passesPerFrame, _drawcallsPerFrame;
		i32 _framesRendered;
		f32 _time, _shadowTime, _geomTime, _accTime, _preTime, _postTime, _2dTime, _frameTime,
			_shadowTimeFrame, _geomTimeFrame, _accTimeFrame, _preTimeFrame, _postTimeFrame, _2dTimeFrame,
			_cpuRenderTime;

		const std::vector<Math::vec3> NDC {
			Math::vec3(-1, -1, -1),
			Math::vec3( 1, -1, -1),
			Math::vec3( 1, -1,  1),
			Math::vec3(-1, -1,  1),

			Math::vec3(-1,  1, -1),
			Math::vec3( 1,  1, -1),
			Math::vec3( 1,  1,  1),
			Math::vec3(-1,  1,  1),
		};
	public:
		Shader* _phongShader, *_shadowShader, *_geomShader, *_guiShader, *_pbrShader;
		ParticleSystem* testPart;
	};
} }

#endif // _BEARISH_GRAPHICS_RENDERINGENGINE_H_