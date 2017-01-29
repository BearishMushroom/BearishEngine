#include "Types.h"
#include "BEMFile.h"
#include <cstdio>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <unordered_map>
#include <Windows.h>

BEMNode AiToBEMNode(aiNode* node) {
	string name = string(node->mName.data);
	mat4 transform = node->mTransformation;
	std::vector<BEMNode> nodes;
	for (i32 i = 0; i < (i32)node->mNumChildren; i++) {
		nodes.push_back(AiToBEMNode(node->mChildren[i]));
	}

	return BEMNode{ name, transform, nodes };
}

struct Vertex {
	vec3 pos;
	vec2 tex;
	vec3 norm;
	vec3 tang;
	vec4i bid;
	vec4 bweight;

	bool operator==(const Vertex const& other) const {
		return pos == other.pos && tex == other.tex && norm == other.norm;
	}
};

namespace std {
	template <> struct hash<vec3> {
		size_t operator()(vec3 const& vec) const {
			return (((u32&)vec.x * 5209) ^ ((u32&)vec.y * 1811)) ^ ((u32&)vec.z * 7297);
		}
	};

	template <> struct hash<vec2> {
		size_t operator()(vec2 const& vec) const {
			return ((u32&)vec.x * 5209) ^ ((u32&)vec.y * 1811);
		}
	};

	template <> struct hash<vec4> {
		size_t operator()(vec4 const& vec) const {
			std::size_t seed = 4;
			for (i32 i = 0; i < 3; i++) {
				seed ^= (size_t)(vec.v[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			}
			return seed;
		}
	};

	template <> struct hash<vec4i> {
		size_t operator()(vec4i const& vec) const {
			std::size_t seed = 4;
			for (i32 i = 0; i < 4; i++) {
				seed ^= (size_t)(vec.v[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			}
			return seed;
		}
	};

	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<vec3>()(vertex.pos) ^
				(hash<vec2>()(vertex.tex) << 1)) >> 1) ^
				(hash<vec3>()(vertex.norm) << 1);
		}
	};
}

void Optimize(BEMFile* file, i32 level, const aiMesh* mesh) {
	aiVector3D zero(0, 0, 0);

	file->numVertices = mesh->mNumVertices;
	file->AllocateVertexData(mesh->HasBones());
	for (u32 i = 0; i < mesh->mNumVertices; i++) {
		const aiVector3D* pos = &(mesh->mVertices[i]);
		const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &zero;
		const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;
		const aiVector3D* tangent = mesh->HasTangentsAndBitangents() ? &(mesh->mTangents[i]) : &zero;

		file->positionData[i] = vec3(pos->x, pos->y, pos->z);
		file->uvData[i] = vec2(texCoord->x, texCoord->y);
		file->normalData[i] = vec3(normal->x, normal->y, normal->z);
		file->tangentData[i] = vec3(tangent->x, tangent->y, tangent->z);
	}

	for (u32 i = 0; i < mesh->mNumBones; i++) {
		u32 boneIndex = 0;
		string boneName(mesh->mBones[i]->mName.data);

		if (file->boneMap.find(boneName) == file->boneMap.end()) {
			boneIndex = file->numBones++;
			file->boneOffsets.push_back(mat4());
			file->boneMap[boneName] = boneIndex;
		}
		else {
			boneIndex = file->boneMap[boneName];
		}

		file->boneOffsets.at(boneIndex) = mat4(mesh->mBones[i]->mOffsetMatrix);

		for (u32 j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			for (u32 id = 0; id < 4; id++) {
				if (file->boneIDData[mesh->mBones[i]->mWeights[j].mVertexId][id] == -1) {
					file->boneIDData[mesh->mBones[i]->mWeights[j].mVertexId][id] = boneIndex;
					file->boneWeightData[mesh->mBones[i]->mWeights[j].mVertexId][id] = mesh->mBones[i]->mWeights[j].mWeight;
				}
			}
		}
	}

	if (level == 0) {
		printf("No mesh optimization done.\n");
		std::vector<u32> indices;
		for (u32 i = 0; i < mesh->mNumFaces; i++) {
			const aiFace& face = mesh->mFaces[i];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		file->numIndices = (i32)indices.size();
		file->indices = new u32[indices.size()];
		memcpy(file->indices, indices.data(), indices.size() * sizeof(u32));
	}

	if (level >= 1) {
		// Remove deplicate vertices
		printf("Removing duplicate vertices...\n");
		if (mesh->mNumFaces > 100) printf("0%%");

		u32 unum = 0;
		std::unordered_map<Vertex, i32> uniques(4096);
		std::vector<Vertex> vertices;
		aiVector3D zero(0, 0, 0);

		vec3* positions = file->positionData;
		vec2* texCoords = file->uvData;
		vec3* norms = file->normalData;
		vec3* tangs = file->tangentData;
		vec4i* bids = file->skinned ? file->boneIDData : 0;
		vec4* bwghs = file->skinned ? file->boneWeightData : 0;

		std::vector<u32> indices;
		for (u32 i = 0; i < mesh->mNumFaces; i++) {
			if (mesh->mNumFaces > 100 && i % (mesh->mNumFaces / 100) == 0) {
				HANDLE hout = GetStdHandle((DWORD)-11);
				CONSOLE_SCREEN_BUFFER_INFO info;
				GetConsoleScreenBufferInfo(hout, &info);
				COORD pos = { 0, info.dwCursorPosition.Y };
				SetConsoleCursorPosition(hout, pos);
				printf("%d%%", (i32)(((f32)i / (f32)mesh->mNumFaces) * 100.f));
			}

			const aiFace& face = mesh->mFaces[i];
			for (u32 j = 0; j < face.mNumIndices; j++) {
				u32 idx = face.mIndices[j];
				Vertex v = {
					positions[idx], texCoords[idx], norms[idx], tangs[idx],
					bids ? bids[idx] : vec4i(-1, -1, -1, -1),
					bwghs ? bwghs[idx] : vec4(0, 0, 0, 0),
				};

				if (uniques.count(v) == 0) {
					uniques[v] = vertices.size();
					vertices.push_back(v);
				}

				indices.push_back(uniques[v]);
			}
		}

		printf("\nMesh optimized from %dv to %dv (%.2f%% of original complexity)\n", file->numVertices, vertices.size(), 
				((f32)vertices.size() / (f32)file->numVertices) * 100.f);

		printf("Resizing mesh data...\n");
		file->numVertices = vertices.size();
		file->AllocateVertexData(mesh->HasBones());
		for (i32 i = 0; i < vertices.size(); i++) {
			Vertex& v = vertices[i];

			file->positionData[i] = v.pos;
			file->uvData[i] = v.tex;
			file->normalData[i] = v.norm;
			file->tangentData[i] = v.tang;
			
			if(mesh->HasBones()) file->boneIDData[i] = v.bid;
			if (mesh->HasBones()) file->boneWeightData[i] = v.bweight;
		}

		file->numIndices = (i32)indices.size();
		file->indices = new u32[indices.size()];
		memcpy(file->indices, indices.data(), indices.size() * sizeof(u32));
	}
}

i32 main(i32 argc, cstring* argv) {
	if (argc < 3) {
		// No input was specified.
		printf("Usage: BEMBuild input.obj output.bem");
		return -1;
	}

	string inputPath = string(argv[1]);
	string outputPath = string(argv[2]);

	u8 opl = 1;

	for (i32 i = 3; i < argc; i++) {
		if (strcmp(argv[i], "-O1") == 0) opl = 1;
		if (strcmp(argv[i], "-O0") == 0) opl = 0;
	}

	BEMFile output;

	Assimp::Importer* importer = new Assimp::Importer;
	const aiScene* scene = importer->ReadFile(inputPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (scene) {
		const aiVector3D zero(0, 0, 0);
		
		auto root = scene->mRootNode;

		for (i32 i = 0; i < (i32)root->mNumChildren; i++) {
			string path = outputPath;
			if (scene->mNumMeshes > 1) {
				string name = path.substr(0, path.find_last_of('.'));
				string ext = path.substr(path.find_last_of('.'));
				path = name + "_" + std::to_string(i) + ext;
			}

			aiMatrix4x4 p = scene->mRootNode->mTransformation;
			const aiMatrix4x4& inv = p.Inverse();

			for (i32 i = 0; i < 4; i++) {
				for (i32 j = 0; j < 4; j++) {
					output.boneTransform[i][j] = inv[i][j];
				}
			}

			const aiMesh* mesh = scene->mMeshes[root->mChildren[i]->mMeshes[0]];
			printf("Processing mesh: %s\n", root->mChildren[i]->mName.C_Str());
			output.name = std::string(root->mChildren[i]->mName.C_Str(), root->mChildren[i]->mName.length);

			output.rootNode = AiToBEMNode(scene->mRootNode);

			std::vector<BEMAnimation> animations;

			for (i32 i = 0; i < (i32)scene->mNumAnimations; i++) {
				aiAnimation* anim = scene->mAnimations[i];
				string name = string(anim->mName.data);
				f32 tickRate = anim->mTicksPerSecond ? (f32)anim->mTicksPerSecond : 30.0f;
				f32 duration = (f32)anim->mDuration;

				std::vector<BEMAnimationChannel> channels;
				for (i32 j = 0; j < (i32)anim->mNumChannels; j++) {
					std::vector<std::pair<vec3, f32>> positions;
					std::vector<std::pair<vec3, f32>> scales;
					std::vector<std::pair<vec4, f32>> rotations;
					string name = string(anim->mChannels[j]->mNodeName.data);

					for (i32 p = 0; p < (i32)anim->mChannels[j]->mNumPositionKeys; p++) {
						aiVector3D aipos = anim->mChannels[j]->mPositionKeys[p].mValue;
						auto key = std::make_pair(vec3(aipos.x, aipos.y, aipos.z), (f32)anim->mChannels[j]->mPositionKeys[p].mTime);
						positions.push_back(key);
					}

					for (i32 s = 0; s < (i32)anim->mChannels[j]->mNumScalingKeys; s++) {
						aiVector3D aiscale = anim->mChannels[j]->mScalingKeys[s].mValue;
						auto key = std::make_pair(vec3(aiscale.x, aiscale.y, aiscale.z), (f32)anim->mChannels[j]->mScalingKeys[s].mTime);
						scales.push_back(key);
					}

					for (i32 r = 0; r < (i32)anim->mChannels[j]->mNumRotationKeys; r++) {
						aiQuaternion airot = anim->mChannels[j]->mRotationKeys[r].mValue;
						auto key = std::make_pair(vec4(airot.x, airot.y, airot.z, airot.w), (f32)anim->mChannels[j]->mRotationKeys[r].mTime);
						rotations.push_back(key);
					}

					channels.push_back(BEMAnimationChannel{ name, (u32)positions.size(), positions, (u32)scales.size(), scales, (u32)rotations.size(), rotations });
				}

				animations.push_back(BEMAnimation{ name, tickRate, duration, (u32)channels.size(), channels });
			}

			output.numAnimations = (i32)animations.size();
			output.animations = animations;

			Optimize(&output, opl, mesh);

			output.WriteToFile(path);
		}
	}
	else {
		printf("Failed to find file %s.", inputPath.c_str());
		return -1;
	}

	return 0;
}