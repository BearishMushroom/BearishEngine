#ifndef _BEARISH_GRAPHICS_RENDERINGENGINE_H_
#define _BEARISH_GRAPHICS_RENDERINGENGINE_H_

#include <BE/Core/IAllocatable.h>
#include <BE/Core/Timer.h>
#include <BE/Types.h>
#include <BE/Graphics/Mesh/Mesh.h>
#include <BE/Graphics/ParticleSystem.h>
#include <BE/Graphics/Font.h>

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

	class BEARISH_API RenderingEngine : public Core::IAllocatable<RenderingEngine> {
	public:
		RenderingEngine();
		~RenderingEngine();

		void Load();
		void Unload();

		void Update();

		void SetActorReference(std::vector<Core::Actor*>* actors);
		void SetCamera(Camera* const camera);

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

		Renderer* GetRenderer();

		u32 GetVertsPerFrame() const { return _vertsPerFrame; }
		u32 GetFacesPerFrame() const { return _facesPerFrame; }
		u32 GetPassesPerFrame() const { return _passesPerFrame; }
		u32 GetDrawcallsPerFrame() const { return _drawcallsPerFrame; }
	private:
		static constexpr i32 SHADOW_NUM_CASCADES = 3;

		std::vector<Light*> _lights;
		Texture* _gbuffer;
		Renderer* _renderer;

		Mesh* _sphere, *_quad;
		Shader* _currentShader;

		Camera* _camera;
		std::vector<Core::Actor*>* _actors;
		i32 _debugMode;

		std::vector<MeshEntry> _meshesToRender;

		Texture* _environmentMap;
		Texture* _preFG;

		Texture* _shadowMap[SHADOW_NUM_CASCADES];
		i32 _shadowCascadeBounds[SHADOW_NUM_CASCADES + 1];
		f32 _shadowMapSize;
		ShadowQuality _shadowQuality;

		Texture* _ssaoNoise,* _ssaoBuffer;
		Shader* _ssaoShader;
		SSAOSetting _ssaoSetting;

		Shader* _blur,* _blur7,* _blur9;

		Font* testFont;

		Core::Timer _timer;

		u32 _vertsPerFrame, _facesPerFrame, _passesPerFrame, _drawcallsPerFrame;
		i32 _framesRendered;

		f32 _time, _shadowTime, _geomTime, _accTime, _preTime, _postTime, _2dTime, _frameTime,
			_shadowTimeFrame, _geomTimeFrame, _accTimeFrame, _preTimeFrame, _postTimeFrame, _2dTimeFrame,
			_cpuRenderTime;

		f32 PointLightSize(PointLight* pl);
		Camera* GetShadowMapProjections(Math::vec3 position, DirectionalLight* light);
		void GenerateShadowMaps();

		Core::Actor* _debugUI;
	public:
		Shader* _shadowShader, *_geomShader, *_guiShader, *_pbrShader;
	};
} }

#endif // _BEARISH_GRAPHICS_RENDERINGENGINE_H_