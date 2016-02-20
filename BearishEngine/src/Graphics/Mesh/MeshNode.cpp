#include "MeshNode.h"
#include "MeshAnimation.h"
#include "MeshSkeleton.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;

void MeshNode::Animate(MeshAnimation* anim, MeshSkeleton* skeleton, f32 time, mat4 parent) {
	MeshNodeAnimation* nodeAnim = anim->GetNodeAnimation(_name);

	mat4 nodeTransform = _transform;

	if (nodeAnim) {
		vec3 scaling = nodeAnim->CalcInterpolatedScale(time);
		mat4 scale = mat4().CreateScale(scaling.x, scaling.y, scaling.z);

		quat rotation = nodeAnim->CalcInterpolatedRotation(time);
		mat4 rotationM = mat4().CreateRotation(rotation.Forward(), rotation.Up(), rotation.Right());

		vec3 translation = nodeAnim->CalcInterpolatedPosition(time);
		mat4 translationM = mat4().CreateTranslation(translation.x, translation.y, translation.z);

		// Combine the above transformations
		nodeTransform = translationM * rotationM * scale;
	}

	mat4 globalTransformation = parent * nodeTransform;

	u32 index = skeleton->GetBoneIndex(_name);
	if (index != -1) {
		anim->SetTransform(index, skeleton->GetTransform() * globalTransformation * skeleton->GetBoneOffset(index));
	}

	for (u32 i = 0; i < _children.size(); i++) {
		_children[i]->Animate(anim, skeleton, time, globalTransformation);
	}
}
