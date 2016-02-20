#ifndef _BEARISH_CORE_IACTORCOMPONENT_H_
#define _BEARISH_CORE_IACTORCOMPONENT_H_

#include "../Scripting/ScriptingEngine.h"
#include "../Graphics/RenderingEngine.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Shader.h"
#include "../Types.h"

namespace Bearish { namespace Core {
	class Actor;

	class IActorComponent {
	public:
		IActorComponent() {}

		template<typename... Args>
		IActorComponent(string squirrelType, Args... args) {
			Sqrat::Function func(Sqrat::RootTable(Scripting::Squirrel), squirrelType.c_str());

			if (!func.IsNull()) {
				_sqThis = func.Evaluate<Sqrat::Object>(args...);
				GetSqFuncs();
			}
		}

		virtual ~IActorComponent() {}

		void GetSqFuncs() {
			_init = Sqrat::Function(*(_sqThis.Get()), "Init");
			_fixed = Sqrat::Function(*(_sqThis.Get()), "FixedUpdate");
			_update = Sqrat::Function(*(_sqThis.Get()), "Update");
			_pre = Sqrat::Function(*(_sqThis.Get()), "PreDraw");
			_draw = Sqrat::Function(*(_sqThis.Get()), "Draw");
			_post = Sqrat::Function(*(_sqThis.Get()), "PostDraw");
			
		}

		virtual void Init() {
			if (_sqThis.Get() && _init.IsNull() == false) {
				_init.Evaluate<i32>();
			}
		}

		virtual void Update() {
			if (_sqThis.Get() && _update.IsNull() == false) {
				_update.Evaluate<i32>();
			}
		}

		virtual void FixedUpdate() {
			if (_sqThis.Get() && _fixed.IsNull() == false) {
				_fixed.Evaluate<i32>();
			}
		}

		virtual void PreDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
			if (_sqThis.Get() && _pre.IsNull() == false) {
				_pre.Evaluate<i32>(renderer, camera);
			}
		}

		virtual void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
			if (_sqThis.Get() && _draw.IsNull() == false) {
				_draw.Evaluate<i32>(renderer, shader, camera);
			}
		}

		virtual void PostDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
			if (_sqThis.Get() && _post.IsNull() == false) {
				_post.Evaluate<i32>(renderer, camera);
			}
		}

		virtual void PreDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
		}

		virtual void Draw2D(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
		}

		virtual void PostDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
		}

		virtual void OnTrigger(string id, void* data) {}

		void SetParent(Actor* actor) {
			_actor = actor;
			if (_sqThis.Get()) {
				_sqThis->Cast<IActorComponent*>()->SetParent(actor);
			}
		}

		Actor* GetParent() {
			return _actor;
		}

		Transform& GetTransform() {
			return _transform;
		}

		string GetID() { return _id; }
		void SetID(string id) { _id = id; }
	protected:
		Actor* _actor;
		Transform _transform;
		string _id;

		Sqrat::SharedPtr<Sqrat::Object> _sqThis;
		string _sqid;
		Sqrat::Function _fixed, _init, _update, _pre, _draw, _post;
	};
} }

#endif
