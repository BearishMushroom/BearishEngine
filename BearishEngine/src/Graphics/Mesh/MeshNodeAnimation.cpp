#include "MeshNodeAnimation.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;

Math::vec3 MeshNodeAnimation::CalcInterpolatedPosition(f32 time) {
	if (positionKeys.size() == 1) {
		return positionKeys.at(0).position;
	}

	u32 posIndex = FindPosition(time);
	u32 nextPosIndex = posIndex + 1;
	f32 deltaTime = positionKeys.at(nextPosIndex).time - positionKeys.at(posIndex).time;
	f32 factor = (time - (f32)positionKeys.at(posIndex).time) / deltaTime;

	vec3 startPos = positionKeys.at(posIndex).position;
	vec3 endPos = positionKeys.at(nextPosIndex).position;
	startPos = startPos.Lerp(endPos, factor);

	return startPos;
}

Math::vec3 MeshNodeAnimation::CalcInterpolatedScale(f32 time) {
	if (scaleKeys.size() == 1) {
		return scaleKeys.at(0).scale;
	}

	u32 scaleIndex = FindScale(time);
	u32 nextScaleIndex = scaleIndex + 1;
	f32 deltaTime = scaleKeys.at(nextScaleIndex).time - scaleKeys.at(scaleIndex).time;
	f32 factor = (time - (f32)scaleKeys.at(scaleIndex).time) / deltaTime;

	vec3 startScale = scaleKeys.at(scaleIndex).scale;
	vec3 endScale = scaleKeys.at(nextScaleIndex).scale;
	startScale = startScale.Lerp(endScale, factor);

	return startScale;
}

Math::quat MeshNodeAnimation::CalcInterpolatedRotation(f32 time) {
	if (rotationKeys.size() == 1) {
		return rotationKeys.at(0).rotation;
	}

	u32 rotIndex = FindRotation(time);
	u32 nextRotIndex = rotIndex + 1;
	f32 deltaTime = rotationKeys.at(nextRotIndex).time - rotationKeys.at(rotIndex).time;
	f32 factor = (time - (f32)rotationKeys.at(rotIndex).time) / deltaTime;

	quat startRot = rotationKeys.at(rotIndex).rotation;
	quat endRot = rotationKeys.at(nextRotIndex).rotation;
	startRot = startRot.NLerp(endRot, factor, true);

	return startRot;
}

u32 MeshNodeAnimation::FindPosition(f32 time) {
	for (u32 i = 0; i < positionKeys.size() - 1; i++) {
		if (time < (f32)positionKeys[i + 1].time) {
			return i;
		}
	}

	return 0;
}

u32 MeshNodeAnimation::FindScale(f32 time) {
	for (u32 i = 0; i < scaleKeys.size() - 1; i++) {
		if (time < (f32)scaleKeys[i + 1].time) {
			return i;
		}
	}

	return 0;
}

u32 MeshNodeAnimation::FindRotation(f32 time) {
	for (u32 i = 0; i < rotationKeys.size() - 1; i++) {
		if (time < (f32)rotationKeys[i + 1].time) {
			return i;
		}
	}

	return 0;
}