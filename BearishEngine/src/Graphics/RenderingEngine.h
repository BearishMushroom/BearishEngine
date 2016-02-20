#ifndef _BEARISH_GRAPHICS_RENDERINGENGINE_H_
#define _BEARISH_GRAPHICS_RENDERINGENGINE_H_

#include "../Core/IAllocatable.h"
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

	struct MeshEntry {
		Mesh* mesh;
		Material* material;

		bool operator==(MeshEntry o) {
			return o.mesh->GetVAOID() == mesh->GetVAOID();
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
		void FlushMeshes(bool bind);
		void SetDebugDeffered(bool b);

		void DrawGuiQuad(Core::Transform t, Texture* tex, u32 subid = 0);

		std::vector<Core::Actor*>* GetActors() { return _actors; }
		Camera* GetCamera() { return _camera; }
		i32 PushMaterial(Material* material);
	private:
		std::vector<Light*> _lights;
		Texture* _shadowMap, *_gbuffer;
		Renderer* _renderer;

		Mesh* _sphere,* _quad;
		Shader* _currentShader;

		Core::Window* _window;
		Camera* _camera;
		std::vector<Core::Actor*>* _actors;
		bool _debugDeffered;

		std::vector<MeshEntry> _meshesToRender;
		std::vector<std::pair<Material*, i32>> _materialsToRender;
		i32 _maxMaterial;

		f32 PointLightSize(PointLight* pl);

		Font* testFont;
	public:
		Shader* _phongShader, *_shadowShader, *_geomShader, *_guiShader;
		ParticleSystem* testPart;
	};
} }

#endif // _BEARISH_GRAPHICS_RENDERINGENGINE_H_