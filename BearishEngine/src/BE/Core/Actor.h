#ifndef _BEARISH_CORE_ACTOR_H_
#define _BEARISH_CORE_ACTOR_H_

#include <BE/Serialization/Serialization.h>
#include <BE/Graphics/Shader.h>
#include <BE/Graphics/Camera.h>
#include <BE\Core\IActorComponent.h>
#include <BE/Math/mat4.h>
#include <BE\Core\IAllocatable.h>
#include <BE\Core\Transform.h>
#include <BE/Types.h>
#include <BE\Core\AABB.h>

namespace Bearish { namespace Core {
	class BEARISH_API Actor /*: public IAllocatable<Actor>*/ {
	public:
		Actor(Transform transform = Transform(), AABB bounds = AABB());
		~Actor();

		template<typename T>
		std::shared_ptr<T> AddComponent(std::shared_ptr<T> component) {
			component->SetParent(this);
			_components.push_back(component);
			component->Init();
			_bounds.Fit(component->GetBounds());
			return component;
		}

		template<typename T>
		std::shared_ptr<T> AddComponent() {
			std::shared_ptr<T> toAdd = std::make_shared<T>;
			AddComponent(toAdd);
			return toAdd;
		}

		template<typename T>
		std::vector<std::shared_ptr<T>> GetComponents() {
			std::vector<std::shared_ptr<T>> result;
			for (auto& c : _components) {
				std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(c);

				if (component) {
					result.push_back(component);
				}
			}

			return result;
		}

		template<typename T>
		std::shared_ptr<T> GetComponent() {
			return GetComponents<T>().at(0);
		}

		std::vector<std::shared_ptr<IActorComponent>> GetComponents(string id);
		std::vector<std::shared_ptr<Actor>> GetChildren() { return _children; }

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
		Math::vec3 GetScale();

		Actor* const GetParent() const;

		bool HasParent() const { return !(_parent == nullptr); }

		void SetParent(Actor* const parent);
		void AddChild(std::shared_ptr<Actor> child);

		bool IsDead() { return !_alive; }
		void Kill() { _alive = false; }

		const AABB& GetBounds() const { return _bounds; }

		template<typename Archive>
		void save(Archive& ar) const {
			ar(CEREAL_NVP(_transform), CEREAL_NVP(_children), CEREAL_NVP(_components));
		}

		template<typename Archive>
		void load(Archive& ar) {
			ar(_transform, _children, _components);
			for (auto& component : _components) {
				component->SetParent(this);
				component->Init();
				_bounds.Fit(component->GetBounds());
			}
		}
	private:
		bool _alive;
		Actor* _parent;
		Transform _transform;
		AABB _bounds;
		std::vector<std::shared_ptr<Actor>> _children;
		std::vector<std::shared_ptr<IActorComponent>> _components;
	};
} }

#endif
