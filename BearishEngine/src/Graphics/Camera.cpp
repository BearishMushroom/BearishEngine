#include "Camera.h"

using namespace Bearish;
using namespace Graphics;
using namespace Core;
using namespace Math;

Camera* Camera::Identity = new Camera(mat4().CreateIdentity(), Transform());


Camera::Camera(const Math::mat4& projection, const Core::Transform& transform) : _transform(transform), _projection(projection) {} 
Camera::~Camera() {}

Core::Transform& Camera::GetTransform() {
	return _transform;
}

Math::mat4 Camera::GetProjection() const {
	return _projection;
}

void Camera::SetTransform(const Core::Transform& value) {
	_transform = value;
}

void Camera::SetProjection(const Math::mat4& value) {
	_projection = value;
}

Math::mat4 Camera::GetViewMatrix() const {
	return _projection * _transform.GetCameraTransformation();
}