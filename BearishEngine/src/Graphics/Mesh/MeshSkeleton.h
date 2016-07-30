#ifndef _BEARISH_GRAPHICS_MESH_SKELETON_H_
#define _BEARISH_GRAPHICS_MESH_SKELETON_H_

#include "MeshBone.h"
#include "../../Types.h"
#include "../../Core/IAllocatable.h"

namespace Bearish { namespace Graphics {
	class MeshSkeleton : public Core::IAllocatable<MeshSkeleton> {
	public:
		MeshSkeleton(std::unordered_map<string, i32> index, std::vector<MeshBone> bones, const Math::mat4& transform) : 
			_indexMap(index), _bones(bones), _numBones((u32)bones.size()), _transform(transform) {}

		inline u32 GetNumBones() { return _numBones; }
		inline u32 GetBoneIndex(string name) { if (_indexMap.find(name) != _indexMap.end()) return _indexMap.at(name); else return -1; }
		inline Math::mat4 GetBoneOffset(u32 index) { return _bones.at(index).offset; }
		inline Math::mat4 GetTransform() { return _transform; }
	private:
		Math::mat4 _transform;
		std::unordered_map<string, i32> _indexMap;
		std::vector<MeshBone> _bones;
		u32 _numBones;
	};
} }

#endif // _BEARISH_GRAPHICS_MESH_SKELETON_H_