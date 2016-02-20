#ifndef _BEARISH_CORE_ACTOR_H_
#define _BEARISH_CORE_ACTOR_H_

#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include "IActorComponent.h"
#include "../Math/mat4.h"
#include "IAllocatable.h"
#include "Transform.h"
#include "../Types.h"

namespace Bearish { namespace Core {
	class Actor : public IAllocatable<Actor> {
	public:
		Actor(Transform transform = Transform());
		~Actor();

		template<typename T>
		T* AddComponent(T* component) {
			component->SetParent(this);
			_components.push_back(component);
			component->Init();
			return component;
		}

		template<typename T>
		T* AddComponent() {
			T* toAdd = new T;
			AddComponent(toAdd);
			return toAdd;
		}

		template<typename T>
		std::vector<T*> GetComponentsByType() {
			std::vector<T*> result;
			for (auto& c : _components) {
				T* component = dynamic_cast<T*>(c);

				if (component) {
					result.push_back(component);
				}
			}

			return result;
		}

		std::vector<IActorComponent*> GetComponentsByID(string id);

		void Trigger(string id, void* data);

		void Update();
		void FixedUpdate();

		void PreDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera);
		void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera);
		void PostDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera);

		void PreDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera);
		void Draw2D(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera);
		void PostDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera);

		Transform& GetTransform();
		Math::mat4 GetTransformation() const;

		Math::vec3 GetTranslation();

		Actor* const GetParent() const;
		void SetParent(Actor* const parent);
		void AddChild(Actor* child);

		bool IsDead() { return !_alive; }
		void Kill() { _alive = false; }
	private:
		bool _alive;
		std::vector<IActorComponent*> _components;
		Transform _transform;
		Actor* _parent;
		std::vector<Actor*> _children;
	};
} }

#endif
