#ifndef _BEARISH_COMPONENTS_LIGHTCOMPONENTS_H_
#define _BEARISH_COMPONENTS_LIGHTCOMPONENTS_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Graphics/Lights.h>

namespace Bearish { namespace Components {
	class BEARISH_API DirectionalLightComponent : public Core::IActorComponent, public Graphics::DirectionalLight {
	public:
		DirectionalLightComponent(const Math::vec3& color, f32 ambientIntensity, f32 diffuseIntensity) :
			DirectionalLight(color, ambientIntensity, Math::quat(0, 0, 0, 1), diffuseIntensity) { _id = "DirectionalLight"; }

		~DirectionalLightComponent() {}

		virtual void PreDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) override {
			_direction = _actor->GetTransform().GetRotation();
			renderer->Submit(this);
		}
	};

	class PointLightComponent : public Core::IActorComponent, public Graphics::PointLight {
	public:
		PointLightComponent(const Math::vec3& color, f32 ambientIntensity, const Graphics::Attenuation& atten, f32 diffuseIntensity) :
			PointLight(color, ambientIntensity, atten, diffuseIntensity, Math::vec3(0)) { _id = "PointLight"; }

		~PointLightComponent() {}

		virtual void PreDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) override {
			_position = _actor->GetTransform().GetTranslation();
			renderer->Submit(this);
		}
	};

	class SpotLightComponent : public Core::IActorComponent, public Graphics::SpotLight {
	public:
		SpotLightComponent(const Math::vec3& color, f32 ambientIntensity, const Graphics::Attenuation& atten, f32 diffuseIntensity, f32 cutoff) :
			SpotLight(color, ambientIntensity, atten, diffuseIntensity, Math::vec3(0), Math::quat(0, 0, 0, 1), cutoff) { _id = "SpotLight"; }

		~SpotLightComponent() {}

		virtual void PreDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) override {
			_direction = _actor->GetTransform().GetRotation();
			_position = _actor->GetTransform().GetTranslation();

			renderer->Submit(this);
		}
	};
} }

#endif // _BEARISH_COMPONENTS_LIGHTCOMPONENTS_H_