#ifndef _BEARISH_CORE_IACTORCOMPONENT_H_
#define _BEARISH_CORE_IACTORCOMPONENT_H_

#include "../Graphics/RenderingEngine.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Shader.h"
#include "../Types.h"

#include "../Scripting/ScriptingEngine.h"

namespace Bearish { namespace Core {
	class Actor;

	class IActorComponent {
	public:
		template<typename... Args>
		IActorComponent(string luaName, Args&&... args) {
			_lua = Scripting::CreateInstance(luaName, args...);
			GetLuaFuncs();
		}

		IActorComponent(string luaName, string luaArgs) {
			_lua = Scripting::CreateInstance(luaName + "(" + luaArgs + ")");
			GetLuaFuncs();
		}

		IActorComponent() {}

		virtual ~IActorComponent() {}

		virtual void Init() {
			if (_hasInit) _lua.CallFunction<void>("Init");
		}

		virtual void Update() {
			if (_hasUpdate) _lua.CallFunction<void>("Update");

		}

		virtual void FixedUpdate() {
			if (_hasFixedUpdate) _lua.CallFunction<void>("FixedUpdate");
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
		Scripting::LuaObject _lua;
		bool _hasInit, _hasUpdate, _hasFixedUpdate, 
			_hasPreDraw, _hasDraw, _hasPostDraw;
	private:
		void GetLuaFuncs() {
			_hasInit = false;
			_hasUpdate = false;
			_hasFixedUpdate = false;
			_hasPreDraw = false;
			_hasDraw = false;
			_hasPostDraw = false;

			if (_lua.Valid()) {
				_hasInit = _lua.HasFunction("Init");
				_hasUpdate = _lua.HasFunction("Update");
				_hasFixedUpdate = _lua.HasFunction("FixedUpdate");
				_hasPreDraw = _lua.HasFunction("PreDraw");
				_hasDraw = _lua.HasFunction("Draw");
				_hasPostDraw = _lua.HasFunction("PostDraw");
			}
		}
	};
} }

#endif
