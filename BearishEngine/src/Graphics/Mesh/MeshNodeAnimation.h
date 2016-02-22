#ifndef _BEARISH_GRAPHICS_MESH_NODE_ANIMATION_H_
#define _BEARISH_GRAPHICS_MESH_NODE_ANIMATION_H_

#include "../../Types.h"
#include "../../Core/IAllocatable.h"
#include "MeshNodeAnimationKeyframe.h"

namespace Bearish { namespace Graphics {
	class MeshNodeAnimation : public Core::IAllocatable<MeshNodeAnimation> {
	public:
		MeshNodeAnimation(string name, std::vector<MeshNodeAnimationKeyframe> pos, std::vector<MeshNodeAnimationKeyframe> scale, std::vector<MeshNodeAnimationKeyframe> rot) :
			name(name), positionKeys(pos), scaleKeys(scale), rotationKeys(rot), nameHash(std::hash<string>()(name)) {}

		Math::vec3 CalcInterpolatedPosition(f32 time);
		Math::vec3 CalcInterpolatedScale(f32 time);
		Math::quat CalcInterpolatedRotation(f32 time);

		u32 FindPosition(f32 time);
		u32 FindScale(f32 time);
		u32 FindRotation(f32 time);

		string name;
		u32 nameHash;
		std::vector<MeshNodeAnimationKeyframe> positionKeys;
		std::vector<MeshNodeAnimationKeyframe> scaleKeys;
		std::vector<MeshNodeAnimationKeyframe> rotationKeys;
	};
} }

#endif // _BEARISH_GRAPHICS_MESH_NODE_ANIMATION_H_
