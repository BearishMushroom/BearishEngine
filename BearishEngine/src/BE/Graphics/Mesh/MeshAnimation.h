#ifndef _BEARISH_GRAPHICS_MESH_ANIMATION_H_
#define _BEARISH_GRAPHICS_MESH_ANIMATION_H_

#include "../../Core/IAllocatable.h"
#include "../../Types.h"
#include "MeshSkeleton.h"
#include "MeshNodeAnimation.h"
#include "MeshNode.h"

namespace Bearish { namespace Graphics {
	class BEARISH_API MeshAnimation : public Core::IAllocatable<MeshAnimation> {
	public:
		MeshAnimation(string name, f32 tickRate, f32 duration, std::vector<MeshNodeAnimation*> channels, MeshSkeleton* skeleton) :
			_active(false), _name(name), _skeleton(skeleton), _tickRate(tickRate), _duration(duration), _time(0), _channels(channels), _nameHash(std::hash<string>()(name)) {}

		void Start();
		void Pause();
		void Stop();
		void SetTime(f32 time);
		void SetTimeScale(f32 scale);
		bool Update(f32 frameTime, MeshNode* root);

		MeshNodeAnimation* GetNodeAnimation(u32 nameHash);
		inline void SetTransform(u32 index, const Math::mat4& t) { _transforms.at(index) = t; }
		inline Math::mat4 GetTransform(u32 index) { return _transforms.at(index); }
		inline string GetName() { return _name; }
		inline u32 GetNameHash() { return _nameHash; }
	private:
		bool _active;
		u32 _nameHash;
		string _name;
		MeshSkeleton* _skeleton;
		f32 _tickRate, _duration, _time, _timeScale;
		std::vector<MeshNodeAnimation*> _channels;
		std::vector<Math::mat4> _transforms;
	};
} }

#endif // _BEARISH_GRAPHICS_MESH_ANIMATION_H_
