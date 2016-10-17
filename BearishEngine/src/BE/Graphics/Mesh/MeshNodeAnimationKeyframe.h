#ifndef _BEARISH_GRAPHICS_MESH_NODE_ANIMATION_KEYFRAME_H_
#define _BEARISH_GRAPHICS_MESH_NODE_ANIMATION_KEYFRAME_H_

#include "../../Types.h"
#include "../../Math/vec3.h"
#include "../../Math/quat.h"
#include "../../Core/IAllocatable.h"

namespace Bearish { namespace Graphics {
	class BEARISH_API MeshNodeAnimationKeyframe : public Core::IAllocatable<MeshNodeAnimationKeyframe> {
	public:
		MeshNodeAnimationKeyframe() {}

		Math::vec3 position;
		Math::vec3 scale;
		Math::quat rotation;
		f32 time;
	};
} }

#endif // _BEARISH_GRAPHICS_MESH_NODE_ANIMATION_KEYFRAME_H_
