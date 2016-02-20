#include "Types.h"
#include "BEMFile.h"
#include <cstdio>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp/scene.h>
#include <vector>

BEMNode AiToBEMNode(aiNode* node) {
	string name = string(node->mName.data);
	mat4 transform = node->mTransformation;
	std::vector<BEMNode> nodes;
	for (i32 i = 0; i < (i32)node->mNumChildren; i++) {
		nodes.push_back(AiToBEMNode(node->mChildren[i]));
	}

	return BEMNode{ name, transform, nodes };
}


i32 main(i32 argc, cstring* argv) {
	if (argc < 3) {
		// No input was specified.
		printf("Usage: BEMBuild input.obj output.bem");
		return -1;
	}

	string inputPath = string(argv[1]);
	string outputPath = string(argv[2]);

	BEMFile output;

	Assimp::Importer* importer = new Assimp::Importer;
	const aiScene* scene = importer->ReadFile(inputPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (scene) {
		const aiVector3D zero(0, 0, 0);
		
		for (i32 i = 0; i < (i32)scene->mNumMeshes; i++) {
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

			const aiMesh* mesh = scene->mMeshes[i];

			output.numVertices = mesh->mNumVertices;
			output.AllocateVertexData(mesh->HasBones());

			for (u32 i = 0; i < mesh->mNumVertices; i++) {
				const aiVector3D* pos = &(mesh->mVertices[i]);
				const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &zero;
				const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;
				const aiVector3D* tangent = mesh->HasTangentsAndBitangents() ? &(mesh->mTangents[i]) : &zero;

				output.positionData[i] = vec3(pos->x, pos->y, pos->z);
				output.uvData[i] = vec2(texCoord->x, texCoord->y);
				output.normalData[i] = vec3(normal->x, normal->y, normal->z);
				output.tangentData[i] = vec3(tangent->x, tangent->y, tangent->z);
			}

			for (u32 i = 0; i < mesh->mNumBones; i++) {
				u32 boneIndex = 0;
				string boneName(mesh->mBones[i]->mName.data);

				if (output.boneMap.find(boneName) == output.boneMap.end()) {
					boneIndex = output.numBones++;
					output.boneOffsets.push_back(mat4());
					output.boneMap[boneName] = boneIndex;
				}
				else {
					boneIndex = output.boneMap[boneName];
				}

				output.boneOffsets.at(boneIndex) = mat4(mesh->mBones[i]->mOffsetMatrix);

				for (u32 j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
					for (u32 id = 0; id < 4; id++) {
						if (output.boneIDData[mesh->mBones[i]->mWeights[j].mVertexId][id] == -1) {
							output.boneIDData[mesh->mBones[i]->mWeights[j].mVertexId][id] = boneIndex;
							output.boneWeightData[mesh->mBones[i]->mWeights[j].mVertexId][id] = mesh->mBones[i]->mWeights[j].mWeight;
						}
					}
				}
			}

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
						auto key = std::make_pair(vec3(aipos.x, aipos.y, aipos.z), anim->mChannels[j]->mPositionKeys[p].mTime);
						positions.push_back(key);
					}

					for (i32 s = 0; s < (i32)anim->mChannels[j]->mNumScalingKeys; s++) {
						aiVector3D aiscale = anim->mChannels[j]->mScalingKeys[s].mValue;
						auto key = std::make_pair(vec3(aiscale.x, aiscale.y, aiscale.z), anim->mChannels[j]->mScalingKeys[s].mTime);
						scales.push_back(key);
					}

					for (i32 r = 0; r < (i32)anim->mChannels[j]->mNumRotationKeys; r++) {
						aiQuaternion airot = anim->mChannels[j]->mRotationKeys[r].mValue;
						auto key = std::make_pair(vec4(airot.x, airot.y, airot.z, airot.w), anim->mChannels[j]->mRotationKeys[r].mTime);
						rotations.push_back(key);
					}

					channels.push_back(BEMAnimationChannel{ name, positions.size(), positions, scales.size(), scales, rotations.size(), rotations });
				}

				animations.push_back(BEMAnimation{ name, tickRate, duration, channels.size(), channels });
			}

			output.numAnimations = animations.size();
			output.animations = animations;

			std::vector<u32> indices;

			for (u32 i = 0; i < mesh->mNumFaces; i++) {
				const aiFace& face = mesh->mFaces[i];
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			output.numIndices = indices.size();
			output.indices = &indices[0];

			output.WriteToFile(path);
		}
	}
	else {
		printf("Failed to find file %s.", inputPath.c_str());
		return -1;
	}

	return 0;
}