#include "Actor.h"

using namespace Bearish;
using namespace Core;
using namespace Graphics;
using namespace Math;

Actor::Actor(Transform transform) {
	_transform = transform;
	_alive = true;
}

Actor::~Actor() {
	for (i32 i = 0; i < (i32)_children.size(); i++) {
		delete _children.at(i);
	}
	_children.clear();

	for (i32 i = 0; i < (i32)_components.size(); i++) {
		delete _components.at(i);
	}
	_components.clear();
}

void Actor::Trigger(string id, void* data) {
	for (auto& a : _children) {
		a->Trigger(id, data);
	}

	for (auto& c : _components) {
		//if (c) {
			c->OnTrigger(id, data);
		//}
	}
}

void Actor::Update() {
	for (auto& a : _children) {
		a->Update();
	}

	for (auto& c : _components) {
		//if (c) {
			c->Update();
		//}
	}
}

void Actor::FixedUpdate() {
	for (auto& a : _children) {
		a->FixedUpdate();
	}

	for (auto& c : _components) {
		//if (c) {
			c->FixedUpdate();
		//}
	}
}

void Actor::PreDraw(Graphics::RenderingEngine* renderer, Camera* camera) {
	for (auto& a : _children) {
		a->PreDraw(renderer, camera);
	}

	for (auto& c : _components) {
		//if (c) {
			c->PreDraw(renderer, camera);
		//}
	}
}

void Actor::Draw(Graphics::RenderingEngine* renderer, Shader* shader, Camera* camera) {
	for (auto& a : _children) {
		a->Draw(renderer, shader, camera);
	}

	for (auto& c : _components) {
		//if (c) {
			c->Draw(renderer, shader, camera);
		//}
	}
}

void Actor::PostDraw(Graphics::RenderingEngine* renderer, Camera* camera) {
	for (auto& a : _children) {
		a->PostDraw(renderer, camera);
	}

	for (auto& c : _components) {
		//if (c) {
			c->PostDraw(renderer, camera);
		//}
	}
}

void Actor::PreDraw2D(Graphics::RenderingEngine* renderer, Camera* camera) {
	for (auto& a : _children) {
		a->PreDraw2D(renderer, camera);
	}

	for (auto& c : _components) {
		//if (c) {
			c->PreDraw2D(renderer, camera);
		//}
	}
}

void Actor::Draw2D(Graphics::RenderingEngine* renderer, Shader* shader, Camera* camera) {
	for (auto& a : _children) {
		a->Draw2D(renderer, shader, camera);
	}

	for (auto& c : _components) {
		//if (c) {
			c->Draw2D(renderer, shader, camera);
		//}
	}
}

void Actor::PostDraw2D(Graphics::RenderingEngine* renderer, Camera* camera) {
	for (auto& a : _children) {
		a->PostDraw2D(renderer, camera);
	}

	for (auto& c : _components) {
		//if (c) {
			c->PostDraw2D(renderer, camera);
		//}
	}
}

mat4 Actor::GetTransformation() const {
	if (_parent) {
		return _parent->GetTransformation() * _transform.GetTransformation();
	}

	return _transform.GetTransformation();
}

Transform& Actor::GetTransform() {
	return _transform;
}

Actor* const Actor::GetParent() const {
	return _parent;
}

void Actor::SetParent(Actor* const parent) {
	_parent = parent;
}

vec3 Actor::GetTranslation() {
	if (_parent) {
		return _parent->GetTranslation() + _transform.GetTranslation();
	}

	return _transform.GetTranslation();
}

vec3 Actor::GetScale() {
	if (_parent) {
		return _parent->GetScale() * _transform.GetScale();
	}

	return _transform.GetScale();
}

std::vector<IActorComponent*> Actor::GetComponentsByID(string id) {
	std::vector<IActorComponent*> result;

	for (auto& c : _components) {
		if (c->GetID() == id) {
			result.push_back(c);
		}
	}

	return result;
}

void Actor::AddChild(Actor* child) {
	_children.push_back(child);
	child->SetParent(this);
}
