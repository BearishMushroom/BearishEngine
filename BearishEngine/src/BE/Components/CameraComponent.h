#ifndef _BEARISH_COMPONENTS_CAMERACOMPONENT_H_
#define _BEARISH_COMPONENTS_CAMERACOMPONENT_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Core/Actor.h>

#include <BE/Graphics/Texture/Texture.h>
#include <BE/Graphics/Mesh/Mesh.h>
#include <BE/Graphics/Camera.h>
#include <BE/Graphics/Shader.h>

#include <BE/Types.h>

namespace Bearish { namespace Components {
	class BEARISH_API CameraComponent : public Core::IActorComponent, public Core::IAllocatable<CameraComponent> {
	public:
		CameraComponent(Graphics::Camera* camera, bool isMain = true) : _camera(camera), _isMain(isMain) { _id = "Camera"; }
		~CameraComponent() { delete _camera; }
	
		virtual void Update() override {
			_camera->SetTransform(_actor->GetTransform());
		}

		virtual void PreDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) override {
			if (_isMain) {
				renderer->SetCamera(_camera);
			}
		}

		void SetMain(bool isMain) { _isMain = isMain; }
		bool GetMain() { return _isMain; }
	private:
		Graphics::Camera* _camera;
		bool _isMain;
	};
} }

#endif // _BEARISH_COMPONENTS_CAMERACOMPONENT_H_