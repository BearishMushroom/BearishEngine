#include "Transform.h"

using namespace Bearish;
using namespace Core;
using namespace Math;

Transform::Transform(vec3 translation, vec3 scale, quat rotation) : _translation(translation), _scale(scale), _rotation(rotation) {}
Transform::~Transform() {}

vec3& Transform::GetTranslation() {
	return _translation;
}

void Transform::SetTranslation(vec3 value) {
	_translation = value;
}

quat& Transform::GetRotation() {
	return _rotation;
}

void Transform::SetRotation(quat value) {
	_rotation = value;
}

vec3& Transform::GetScale() {
	return _scale;
}

void Transform::SetScale(vec3 value) {
	_scale = value;
}

void Transform::Rotate(const vec3& axis, f32 angle) {
	SetRotation(GetRotation() * quat().CreateRotation(axis, angle));
}

void Transform::Rotate(const quat& rotation) {
	_rotation = quat((_rotation * rotation).Normalize());
}

void Transform::LookAt(const vec3& point, const vec3& up) {
	_rotation = quat::GetLookAtDirection(_translation, point, up);
}

mat4 Transform::GetTransformation() const {
	mat4 translation = mat4().CreateTranslation(_translation.x, _translation.y, _translation.z);
	mat4 rotation = mat4().CreateRotation(_rotation.Forward(), _rotation.Up(), _rotation.Right());
	mat4 scale = mat4().CreateScale(_scale.x, _scale.y, _scale.z);

	return translation * (rotation * scale);
}

mat4 Transform::GetCameraTransformation() const {
	mat4 translation = mat4().CreateTranslation(-_translation.x, -_translation.y, -_translation.z);
	mat4 rotation = mat4().CreateRotation(_rotation.Forward(), _rotation.Up(), _rotation.Right());
	
	return rotation * translation;
}