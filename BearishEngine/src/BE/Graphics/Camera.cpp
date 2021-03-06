#include "Camera.h"

using namespace Bearish;
using namespace Graphics;
using namespace Core;
using namespace Math;

Camera* Camera::Identity = new Camera(mat4().CreateIdentity(), Transform());


Camera::Camera(const f32 fov, const f32 aspect, const f32 nearB, const f32 farB, const Core::Transform& transform) : _transform(transform), 
				_projection(mat4().CreateProjection(fov, aspect, nearB, farB)), _near(nearB), _far(farB), _aspect(aspect), _fov(fov) {}

Camera::Camera(const mat4& proj, const Core::Transform& transform) : _transform(transform), _projection(proj) {}


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