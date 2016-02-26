#ifndef _BEARISH_CORE_IACTORCOMPONENT_H_
#define _BEARISH_CORE_IACTORCOMPONENT_H_

#include "../Graphics/RenderingEngine.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Shader.h"
#include "../Types.h"

namespace Bearish { namespace Core {
	class Actor;

	class IActorComponent {
	public:
		IActorComponent() {}

		virtual ~IActorComponent() {}

		virtual void Init() {
		}

		virtual void Update() {
		}

		virtual void FixedUpdate() {
		}

		virtual void PreDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
		}

		virtual void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
		}

		virtual void PostDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
		}

		virtual void PreDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
		}

		virtual void Draw2D(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
		}

		virtual void PostDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
		}

		virtual void OnTrigger(string id, void* data) {}

		void SetParent(Actor* actor) { _actor = actor; }
		Actor* GetParent() { return _actor; }
		Transform& GetTransform() {	return _transform; }

		string GetID() { return _id; }
		void SetID(string id) { _id = id; }
	protected:
		Actor* _actor;
		Transform _transform;
		string _id;
	};
} }

#endif
