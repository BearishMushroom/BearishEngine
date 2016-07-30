#include "Transform.h"

using namespace Bearish;
using namespace Core;
using namespace Math;

Transform::Transform(vec3 translation, vec3 scale, quat rotation) : _translation(translation), _scale(scale), _rotation(rotation) {}

Transform::Transform(const mat4& transformation) {
	_translation = vec3(transformation[0][3], transformation[1][3], transformation[2][3]);
	
	f32 x = vec3(transformation[0][0], transformation[0][1], transformation[0][2]).Length();
	f32 y = vec3(transformation[1][0], transformation[1][1], transformation[1][2]).Length();
	f32 z = vec3(transformation[2][0], transformation[2][1], transformation[2][2]).Length();
	
	vec3 neg = vec3(transformation[0][0], transformation[0][1], transformation[0][2])
		.Cross(vec3(transformation[1][0], transformation[1][1], transformation[1][2]));

	if (neg.Dot(vec3(transformation[2][0], transformation[2][1], transformation[2][2])) < 0) {
		x = -x;
	}

	_scale = vec3(x, y, z);

	_rotation = quat(transformation);
}

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