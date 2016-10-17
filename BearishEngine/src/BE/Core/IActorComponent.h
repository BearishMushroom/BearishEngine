#ifndef _BEARISH_CORE_IACTORCOMPONENT_H_
#define _BEARISH_CORE_IACTORCOMPONENT_H_

#include <BE/Graphics/RenderingEngine.h>
#include <BE/Graphics/Camera.h>
#include <BE/Graphics/Shader.h>
#include <BE/Types.h>

#include <BE/Scripting/ScriptingEngine.h>

namespace Bearish { namespace Core {
	class Actor;

	class BEARISH_API IActorComponent {
	public:
		template<typename... Args>
		IActorComponent(string luaName, Args&&... args) {
			_lua = Scripting::CreateInstance(luaName, args...);
			_id = luaName;
			GetLuaFuncs();
		}

		IActorComponent(string luaName) {
			_lua = Scripting::CreateInstance(luaName + "()");
			_id = luaName;
			GetLuaFuncs();
		}

		IActorComponent(string luaName, string luaArgs) {
			_lua = Scripting::CreateInstance(luaName + "(" + luaArgs + ")");
			_id = luaName;
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
			if (_hasPreDraw) _lua.CallFunction<void>("PreDraw", renderer, camera);
		}

		virtual void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
			if (_hasDraw) _lua.CallFunction<void>("Draw", renderer, shader, camera);
		}

		virtual void PostDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
			if (_hasPostDraw) _lua.CallFunction<void>("PostDraw", renderer, camera);
		}

		virtual void PreDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
			if (_hasPreDraw2D) _lua.CallFunction<void>("PreDraw2D", renderer, camera);
		}

		virtual void Draw2D(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
			if (_hasDraw2D) _lua.CallFunction<void>("Draw2D", renderer, shader, camera);
		}

		virtual void PostDraw2D(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) {
			if (_hasPostDraw2D) _lua.CallFunction<void>("PostDraw2D", renderer, camera);
		}

		virtual Math::vec3 GetBounds() {
			if (_hasGetBounds) return _lua.CallFunction<Math::vec3>("GetBounds");
			return vec3(0);
		}

		virtual void OnTrigger(string id, void* data) {}

		void SetParent(Actor* actor) { _actor = actor; if (_lua.Valid()) _lua.Set("actor", actor); }
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
			_hasPreDraw, _hasDraw, _hasPostDraw,
			_hasPreDraw2D, _hasDraw2D, _hasPostDraw2D,
			_hasGetBounds;
	private:
		void GetLuaFuncs() {
			if (_lua.Valid()) {
				_hasInit = _lua.HasFunction("Init");
				_hasUpdate = _lua.HasFunction("Update");
				_hasFixedUpdate = _lua.HasFunction("FixedUpdate");
				_hasPreDraw = _lua.HasFunction("PreDraw");
				_hasDraw = _lua.HasFunction("Draw");
				_hasPostDraw = _lua.HasFunction("PostDraw");
				_hasPreDraw2D = _lua.HasFunction("PreDraw2D");
				_hasDraw2D = _lua.HasFunction("Draw2D");
				_hasPostDraw2D = _lua.HasFunction("PostDraw2D");
				_hasGetBounds = _lua.HasFunction("GetBounds");
			}
		}
	};
} }

#endif
