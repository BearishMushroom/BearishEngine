#ifndef _BEARISH_SCRIPTING_CORE_H_
#define _BEARISH_SCRIPTING_CORE_H_

#include "../Core/IActorComponent.h"
#include "ScriptingEngine.h"
#include "../Core/Actor.h"
#include "../Types.h"

namespace Bearish { namespace Scripting {
	static void RegisterCore() {
		Sqrat::Class<Core::Transform> transform(Squirrel, "Transform", true);
		transform.Ctor<Math::vec3, Math::vec3, Math::quat>();
		
		transform.Func("GetTranslation", &Core::Transform::GetTranslation);
		transform.Func("GetScale", &Core::Transform::GetScale);
		transform.Func("GetRotation", &Core::Transform::GetRotation);

		transform.Func("SetTranslation", &Core::Transform::SetTranslation);
		transform.Func("SetScale", &Core::Transform::SetScale);
		transform.Func("SetRotation", &Core::Transform::SetRotation);

		transform.Func("GetTransformation", &Core::Transform::GetTransformation);
		transform.Func("GetCameraTransformation", &Core::Transform::GetCameraTransformation);

		transform.Func("Rotate", static_cast<void(Core::Transform::*)(const Math::vec3&, const f32)>(&Core::Transform::Rotate));

		Sqrat::RootTable(Squirrel).Bind("Transform", transform);

		Sqrat::Class<Core::IActorComponent> component(Squirrel, "IActorComponent", true);
		component.Func("Init", &Core::IActorComponent::Init);
		component.Func("Update", &Core::IActorComponent::Update);
		component.Func("FixedUpdate", &Core::IActorComponent::FixedUpdate);
		component.Func("PreDraw", &Core::IActorComponent::PreDraw);
		component.Func("Draw", &Core::IActorComponent::Draw);
		component.Func("PostDraw", &Core::IActorComponent::PostDraw);
		//component.Func("OnTrigger", &Core::IActorComponent::OnTrigger);
		component.Func("GetID", &Core::IActorComponent::GetID);
		component.Func("SetID", &Core::IActorComponent::SetID);
		component.Func("GetParent", &Core::IActorComponent::GetParent);
		component.Func("SetParent", &Core::IActorComponent::SetParent);
		
		Sqrat::RootTable(Squirrel).Bind("IActorComponent", component);

		Sqrat::Class<Core::Actor> actor(Squirrel, "Actor", true);
		actor.Ctor<Core::Transform>();
		//actor.Func("AddComponent", &Core::Actor::AddComponent);
		actor.Func("GetComponentsByID", &Core::Actor::GetComponentsByID);
		actor.Func("Update", &Core::Actor::Update);
		actor.Func("FixedUpdate", &Core::Actor::FixedUpdate);
		actor.Func("GetTransform", &Core::Actor::GetTransform);
		actor.Func("GetTransformation", &Core::Actor::GetTransformation);
		actor.Func("GetParent", &Core::Actor::GetParent);
		actor.Func("SetParent", &Core::Actor::SetParent);
		Sqrat::RootTable(Squirrel).Bind("Actor", actor);
	}
} }

#endif // _BEARISH_SCRIPTING_CORE_H_