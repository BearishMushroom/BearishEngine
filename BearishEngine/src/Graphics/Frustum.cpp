#include "Frustum.h"

using namespace Bearish;
using namespace Core;
using namespace Graphics;
using namespace Math;

Frustum::Frustum(const mat4& mvp) {
	_planes[(i32)Plane::Near].Set(
		mvp[3][1] + mvp[4][1],
		mvp[3][2] + mvp[4][2],
		mvp[3][3] + mvp[4][3],
		mvp[3][4] + mvp[4][4]);

	_planes[(i32)Plane::Far].Set(
		-mvp[3][1] + mvp[4][1],
		-mvp[3][2] + mvp[4][2],
		-mvp[3][3] + mvp[4][3],
		-mvp[3][4] + mvp[4][4]);

	_planes[(i32)Plane::Bottom].Set(
		mvp[2][1] + mvp[4][1],
		mvp[2][2] + mvp[4][2],
		mvp[2][3] + mvp[4][3],
		mvp[2][4] + mvp[4][4]);

	_planes[(i32)Plane::Top].Set(
		-mvp[2][1] + mvp[4][1],
		-mvp[2][2] + mvp[4][2],
		-mvp[2][3] + mvp[4][3],
		-mvp[2][4] + mvp[4][4]);

	_planes[(i32)Plane::Left].Set(
		mvp[1][1] + mvp[4][1],
		mvp[1][2] + mvp[4][2],
		mvp[1][3] + mvp[4][3],
		mvp[1][4] + mvp[4][4]);

	_planes[(i32)Plane::Right].Set(
		-mvp[1][1] + mvp[4][1],
		-mvp[1][2] + mvp[4][2],
		-mvp[1][3] + mvp[4][3],
		-mvp[1][4] + mvp[4][4]);
}

bool Frustum::PointIntersects(const vec3& point) const {
	for (i32 i = 0; i < 6; i++)
		if (_planes[i].Distance(point) < 0)
			return false;

	return true;
}

bool Frustum::SphereIntersects(const vec3& pos, f32 rad) const {
	f32 distance;

	for (i32 i = 0; i < 6; i++) {
		distance = _planes[i].Distance(pos);
		if (distance < -rad)
			return false;
	}

	return true;
}