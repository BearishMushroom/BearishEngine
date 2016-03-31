#include "../Math/vec2.h"
#include "../Math/vec3.h"
#include "Model.h"
#include "Timer.h"

#include "../Graphics/Mesh/MeshNodeAnimationKeyframe.h"
#include "../Graphics/Mesh/MeshNodeAnimation.h"
#include "../Graphics/Mesh/MeshAnimation.h"
#include "../Graphics/Mesh/MeshSkeleton.h"
#include "../Graphics/Mesh/MeshBone.h"
#include "../Graphics/Mesh/MeshNode.h"
#include "../Graphics/Mesh/Mesh.h"

using namespace Bearish;
using namespace Core;
using namespace Graphics;
using namespace Math;

std::unordered_map<string, BEMFile*> Model::_dataMap;

Model::Model(const string& filename) {
	_filename = filename;

	Timer timer;
	timer.Start();
	_data = new BEMFile();
	_data->ReadFromFile(filename);
	Logger::Info("Model %s loaded in %.3f ms", filename.c_str(), timer.DeltaMS());
}

Model::~Model() {
	delete _data;
}

Mesh Model::ToMesh() {
	vec4i* bid = 0;
	vec4* weight = 0;
	if (_data->skinned) {
		bid = _data->boneIDData;
		weight = _data->boneWeightData;
	}

	Mesh m(_data->numVertices, _data->positionData, _data->uvData, _data->normalData, _data->tangentData, bid, weight, _data->numIndices, _data->indices);
	std::vector<MeshBone> bones;
	for (auto b : _data->boneOffsets) {
		bones.push_back(MeshBone{ b, mat4() });
	}

	MeshNode* rootNode = BEMToBENode(_data->rootNode);
	MeshSkeleton* skeleton = new MeshSkeleton(_data->boneMap, bones, _data->boneTransform);
	
	std::vector<MeshAnimation> animations;
	for (i32 i = 0; i < (i32)_data->numAnimations; i++) {
		BEMAnimation anim = _data->animations[i];
		string name = anim.name;
		f32 tickRate = anim.tickRate;
		f32 duration = anim.duration;

		std::vector<MeshNodeAnimation*> channels;
		for (i32 j = 0; j < (i32)anim.numChannels; j++) {
			std::vector<MeshNodeAnimationKeyframe> positions;
			std::vector<MeshNodeAnimationKeyframe> scales;
			std::vector<MeshNodeAnimationKeyframe> rotations;
			string name = string(anim.channels[j].name);

			for (i32 p = 0; p < (i32)anim.channels[j].numPositions; p++) {
				MeshNodeAnimationKeyframe key;
				key.position = anim.channels[j].positions[p].first;
				key.time = anim.channels[j].positions[p].second;
				positions.push_back(key);
			}

			for (i32 s = 0; s < (i32)anim.channels[j].numScales; s++) {
				MeshNodeAnimationKeyframe key;
				key.scale = anim.channels[j].scales[s].first;
				key.time = anim.channels[j].scales[s].second;
				scales.push_back(key);
			}

			for (i32 r = 0; r < (i32)anim.channels[j].numRotations; r++) {
				MeshNodeAnimationKeyframe key;
				key.rotation = quat(anim.channels[j].rotations[r].first.x, anim.channels[j].rotations[r].first.y,
					anim.channels[j].rotations[r].first.z, anim.channels[j].rotations[r].first.w);

				key.time = anim.channels[j].rotations[r].second;
				rotations.push_back(key);
			}

			channels.push_back(new MeshNodeAnimation(name, positions, scales, rotations));
		}

		animations.push_back(MeshAnimation(name, tickRate, duration, channels, skeleton));
	}

	m.SetBoneData(skeleton, rootNode, animations, _data->boneTransform);
	return m;
}

MeshNode* Model::BEMToBENode(BEMNode node) {
	string name = string(node.name);
	mat4 transform = node.transform;
	std::vector<MeshNode*> nodes;
	for (i32 i = 0; i < (i32)node.children.size(); i++) {
		nodes.push_back(BEMToBENode(node.children[i]));
	}

	return new MeshNode(transform, nodes, name);
}
