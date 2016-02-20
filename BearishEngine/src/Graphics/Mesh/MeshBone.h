#ifndef _BEARISH_GRAPHICS_MESH_BONE_H_
#define _BEARISH_GRAPHICS_MESH_BONE_H_

#include "../../Types.h"
#include "../../Math/mat4.h"
#include "../../Core/IAllocatable.h"

namespace Bearish { namespace Graphics {
	class MeshBone : public Core::IAllocatable<MeshBone> {
	public:
		MeshBone(Math::mat4 offset, Math::mat4 transform) : offset(offset), transform(transform) {}
		Math::mat4 offset, transform;
	};
} }

#endif // _BEARISH_GRAPHICS_MESH_BONE_H_
