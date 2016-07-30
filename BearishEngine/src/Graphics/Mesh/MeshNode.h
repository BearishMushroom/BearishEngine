#ifndef _BEARISH_GRAPHICS_MESH_NODE_H_
#define _BEARISH_GRAPHICS_MESH_NODE_H_

#include "../../Types.h"
#include "../../Core/IAllocatable.h"
#include "../../Math/mat4.h"

namespace Bearish { namespace Graphics {
	class MeshAnimation;
	class MeshSkeleton;

	class MeshNode : public Core::IAllocatable<MeshNode> {
	public:
		MeshNode(const Math::mat4& transform, std::vector<MeshNode*> nodes, string name) : _transform(transform), _children(nodes), 
			_name(name), _nameHash(std::hash<string>()(name)) {}

		void Animate(MeshAnimation* anim, MeshSkeleton* skeleton, f32 time, const Math::mat4& parent = Math::mat4().CreateIdentity());
	private:
		Math::mat4 _transform;
		std::vector<MeshNode*> _children;
		string _name;
		u32 _nameHash;
	};
} }

#endif // _BEARISH_GRAPHICS_MESH_NODE_H_
