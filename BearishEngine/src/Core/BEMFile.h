#pragma once

#include "../Types.h"
#include "../Math/vec2.h"
#include "../Math/vec3.h"
#include "../Math/vec4.h"
#include "../Math/mat4.h"
#include <stdio.h>
#include <cassert>
#include <unordered_map>
#include <vector>

/*

.bem BearishEngine Model format

Header: 4 bytes
'B''E''M'[version]

Number of vertices: 4 bytes
Number of indices: 4 bytes
Is skinned: 1 byte

Index data: number of indices * sizeof(i32)

Position data: number of vertices * sizeof(f32) * 3
UV data: number of vertices * sizeof(f32) * 2
Normal data: number of vertices * sizeof(f32) * 3
Tangent data: number of vertices * sizeof(f32) * 3

if Is skinned:
	Bone ID data: number of vertices * sizeof(i32) * 4
	Bone weight data: number of vertices * sizeof(f32) * 4

	mat4 bonetransform
	u32 numberofbones

	numberofbones times ->
		u8 nameLength
		char* name
		i32 index
		mat4 offset

	fn readnode ->
		u8 namelength
		char* name
		mat4 transform
		u8 numchildren
		for(numchildren) ->
			readnode

	u32 numanimations
	numanimations times ->
		u8 namelength
		char* name
		f32 tickrate
		f32 duration
		u32 numchannels
		numchannels times ->
			u8 namelength
			char* name

			u32 numpos
			numpos times ->
				vec3 pos
				f32 time

			u32 numscale
			numscale times ->
				vec3 scale
				f32 time

			u32 numrot
			numrot times ->
				quat rot
				f32 time
*/

namespace Bearish {
	using namespace Math;

	struct BEMNode {
		string name;
		mat4 transform;
		std::vector<BEMNode> children;
	};

	struct BEMAnimationChannel {
		string name;
		u32 numPositions;
		std::vector<std::pair<vec3, f32>> positions;

		u32 numScales;
		std::vector<std::pair<vec3, f32>> scales;

		u32 numRotations;
		std::vector<std::pair<vec4, f32>> rotations;
	};

	struct BEMAnimation {
		string name;
		f32 tickRate;
		f32 duration;
		u32 numChannels;
		std::vector<BEMAnimationChannel> channels;
	};
	class BEMFile {
	public:
		u8 header[4];
		i32 numVertices = 0;
		i32 numIndices = 0;
		u8 skinned = false;
		std::string name;

		u32* indices;

		vec3* positionData;
		vec2* uvData;
		vec3* normalData;
		vec3* tangentData;

		vec4i* boneIDData;
		vec4* boneWeightData;

		mat4 boneTransform;
		i32 numBones;
		std::unordered_map<string, i32> boneMap;
		std::vector<mat4> boneOffsets;
		BEMNode rootNode;

		i32 numAnimations;
		std::vector<BEMAnimation> animations;

		u32 bytesWritten;

		BEMFile() {
			header[0] = 'B';
			header[1] = 'E';
			header[2] = 'M';
			header[3] = 2;
		}

		~BEMFile() {
			free(indices);
			free(positionData);
			free(uvData);
			free(normalData);
			free(tangentData);
			if (skinned) {
				free(boneIDData);
				free(boneWeightData);
			}
		}

		void AllocateVertexData(bool skinned) {
			positionData = new vec3[numVertices];
			uvData = new vec2[numVertices];
			normalData = new vec3[numVertices];
			tangentData = new vec3[numVertices];
			if (skinned) {
				this->skinned = skinned;
				boneIDData = new vec4i[numVertices];
				boneWeightData = new vec4[numVertices];
				memset(boneIDData, -1, numVertices * sizeof(vec4i));
				memset(boneWeightData, 0, numVertices * sizeof(vec4));
			}
		}

		void WriteBytes(FILE* file, u8* bytes, u32 size, u32 element) {
			fwrite(bytes, element, size, file);
			bytesWritten += size * element;
		}

		u8* ReadBytes(FILE* file, u32 size, u32 element) {
			u8* buffer = (u8*)malloc(size * element);
			fread(buffer, element, size, file);
			return buffer;
		}

		void WriteNode(FILE* file, BEMNode node) {
			u8 len = (u8)node.name.length();
			i32 num = (i32)node.children.size();
			WriteBytes(file, &len, 1, sizeof(u8));
			WriteBytes(file, (u8*)node.name.c_str(), len, sizeof(u8));
			WriteBytes(file, (u8*)&node.transform, 1, sizeof(mat4));
			WriteBytes(file, (u8*)&num, 1, sizeof(i32));
			for (i32 i = 0; i < num; i++) {
				WriteNode(file, node.children[i]);
			}
		}

		BEMNode ReadNode(FILE* file) {
			BEMNode res;
			u8 len = *ReadBytes(file, 1, sizeof(u8));
			res.name = string((char*)ReadBytes(file, len, sizeof(u8)), len);
			res.transform = *(mat4*)ReadBytes(file, 1, sizeof(mat4));
			i32 num = *(i32*)ReadBytes(file, 1, sizeof(i32));
			for (i32 i = 0; i < num; i++) {
				res.children.push_back(ReadNode(file));
			}
			return res;
		}

		void WriteToFile(string filename) {
			bytesWritten = 0;
			FILE* output;
			fopen_s(&output, filename.c_str(), "wb");

			printf("Writing %d vertices and %d indices to %s.\n", numVertices, numIndices, filename.c_str());

			printf("Writing header...                 ");
			WriteBytes(output, header, 4, 1);
			WriteBytes(output, (u8*)&numVertices, 1, sizeof(u32));
			WriteBytes(output, (u8*)&numIndices, 1, sizeof(u32));
			WriteBytes(output, (u8*)&skinned, 1, sizeof(u8));
			u8 nameLength = name.length();
			WriteBytes(output, &nameLength, 1, sizeof(u8));
			WriteBytes(output, (u8*)name.c_str(), 1, sizeof(u8) * nameLength);
			printf("%d\n", bytesWritten);

			u32 preBytes = bytesWritten;
			printf("Writing indices...                ");
			WriteBytes(output, (u8*)indices, numIndices, sizeof(u32));
			printf("%d\n", bytesWritten - preBytes);

			preBytes = bytesWritten;
			printf("Writing vertices...               ");
			WriteBytes(output, (u8*)positionData, numVertices, sizeof(vec3));
			WriteBytes(output, (u8*)uvData, numVertices, sizeof(vec2));
			WriteBytes(output, (u8*)normalData, numVertices, sizeof(vec3));
			WriteBytes(output, (u8*)tangentData, numVertices, sizeof(vec3));
			printf("%d\n", bytesWritten - preBytes);

			if (skinned) {
				preBytes = bytesWritten;
				printf("Writing bone data...              ");
				WriteBytes(output, (u8*)boneIDData, numVertices, sizeof(vec4i));
				WriteBytes(output, (u8*)boneWeightData, numVertices, sizeof(vec4));
				printf("%d\n", bytesWritten - preBytes);

				preBytes = bytesWritten;
				printf("Writing skeleton data...          ");
				WriteBytes(output, (u8*)&boneTransform, 1, sizeof(mat4));
				WriteBytes(output, (u8*)&numBones, 1, sizeof(u32));
				for (auto&& it : boneMap) {
					u8 length = (u8)it.first.length();

					WriteBytes(output, &length, 1, sizeof(u8));
					WriteBytes(output, (u8*)it.first.c_str(), length, sizeof(u8));
					WriteBytes(output, (u8*)&it.second, 1, sizeof(i32));
					WriteBytes(output, (u8*)&boneOffsets[it.second], 1, sizeof(mat4));
				}
				printf("%d\n", bytesWritten - preBytes);

				preBytes = bytesWritten;
				printf("Writing node data...              ");
				WriteNode(output, rootNode);
				printf("%d\n", bytesWritten - preBytes);

				preBytes = bytesWritten;
				printf("Writing animation data...         ");
				WriteBytes(output, (u8*)&numAnimations, 1, sizeof(u32));

				for (i32 i = 0; i < numAnimations; i++) {
					u8 len = (u8)animations[i].name.length();
					WriteBytes(output, &len, 1, sizeof(u8));
					WriteBytes(output, (u8*)animations[i].name.c_str(), len, sizeof(u8));
					WriteBytes(output, (u8*)&animations[i].tickRate, 1, sizeof(f32));
					WriteBytes(output, (u8*)&animations[i].duration, 1, sizeof(f32));
					WriteBytes(output, (u8*)&animations[i].numChannels, 1, sizeof(u32));

					for (i32 j = 0; j < (i32)animations[i].numChannels; j++) {
						BEMAnimationChannel ch = animations[i].channels[j];
						u8 length = (u8)ch.name.length();
						WriteBytes(output, &length, 1, sizeof(u8));
						WriteBytes(output, (u8*)ch.name.c_str(), length, sizeof(u8));

						WriteBytes(output, (u8*)&ch.numPositions, 1, sizeof(u32));
						WriteBytes(output, (u8*)&ch.positions[0], ch.numPositions, sizeof(std::pair<vec3, f32>));

						WriteBytes(output, (u8*)&ch.numScales, 1, sizeof(u32));
						WriteBytes(output, (u8*)&ch.scales[0], ch.numScales, sizeof(std::pair<vec3, f32>));

						WriteBytes(output, (u8*)&ch.numRotations, 1, sizeof(u32));
						WriteBytes(output, (u8*)&ch.rotations[0], ch.numRotations, sizeof(std::pair<vec4, f32>));
					}
				}
				printf("%d\n", bytesWritten - preBytes);
			}

			fclose(output);

			printf("%d bytes written.\n", bytesWritten);
		}

		void ReadFromFile(string filename) {
			FILE* file;
			fopen_s(&file, filename.c_str(), "rb");

			u8* header = ReadBytes(file, 4, 1);
			assert(memcmp(header, this->header, 3) == 0);

			numVertices = *(u32*)ReadBytes(file, 4, 1);
			numIndices = *(u32*)ReadBytes(file, 4, 1);

			skinned = *(bool*)ReadBytes(file, 1, 1);

			if (header[3] > 1) {
				u8 nameLength = *ReadBytes(file, 1, 1);
				name = std::string((const char*)ReadBytes(file, nameLength, 1), nameLength);
			}

			indices = (u32*)ReadBytes(file, numIndices, sizeof(u32));

			positionData = (vec3*)ReadBytes(file, numVertices * 3, 4);
			uvData = (vec2*)ReadBytes(file, numVertices * 2, 4);
			normalData = (vec3*)ReadBytes(file, numVertices * 3, 4);
			tangentData = (vec3*)ReadBytes(file, numVertices * 3, 4);


			if (skinned) {
				boneIDData = (vec4i*)ReadBytes(file, numVertices, sizeof(vec4i));
				boneWeightData = (vec4*)ReadBytes(file, numVertices, sizeof(vec4));

				boneTransform = *(mat4*)ReadBytes(file, 1, sizeof(mat4));
				numBones = *(u32*)ReadBytes(file, 1, sizeof(u32));
				boneOffsets.resize(numBones);

				for (i32 i = 0; i < numBones; i++) {
					u8 len = *ReadBytes(file, 1, sizeof(u8));
					string name = string((char*)ReadBytes(file, len, sizeof(u8)), len);
					u32 id = *(u32*)ReadBytes(file, 1, sizeof(u32));
					mat4 offset = *(mat4*)ReadBytes(file, 1, sizeof(mat4));
					boneOffsets[id] = offset;
					boneMap.insert(std::make_pair(name, id));
				}

				rootNode = ReadNode(file);

				numAnimations = *(u32*)ReadBytes(file, 1, sizeof(u32));
				for (i32 i = 0; i < numAnimations; i++) {
					BEMAnimation anim;
					u8 len = *ReadBytes(file, 1, sizeof(u8));
					anim.name = string((char*)ReadBytes(file, len, sizeof(u8)), len);
					anim.tickRate = *(f32*)ReadBytes(file, 1, sizeof(f32));
					anim.duration = *(f32*)ReadBytes(file, 1, sizeof(f32));
					anim.numChannels = *(u32*)ReadBytes(file, 1, sizeof(u32));

					for (i32 j = 0; j < (i32)anim.numChannels; j++) {
						BEMAnimationChannel ch;
						u8 length = *ReadBytes(file, 1, sizeof(u8));
						ch.name = string((char*)ReadBytes(file, length, sizeof(u8)), length);

						ch.numPositions = *(u32*)ReadBytes(file, 1, sizeof(u32));
						auto pos = (std::pair<vec3, f32>*)ReadBytes(file, ch.numPositions, sizeof(std::pair<vec3, f32>));
						ch.positions = std::vector<std::pair<vec3, f32>>(pos, pos + ch.numPositions);

						ch.numScales = *(u32*)ReadBytes(file, 1, sizeof(u32));
						auto scale = (std::pair<vec3, f32>*)ReadBytes(file, ch.numScales, sizeof(std::pair<vec3, f32>));
						ch.scales = std::vector<std::pair<vec3, f32>>(scale, scale + ch.numScales);

						ch.numRotations = *(u32*)ReadBytes(file, 1, sizeof(u32));
						auto rot = (std::pair<vec4, f32>*)ReadBytes(file, ch.numRotations, sizeof(std::pair<vec4, f32>));
						ch.rotations = std::vector<std::pair<vec4, f32>>(rot, rot + ch.numRotations);
						anim.channels.push_back(ch);
					}

					animations.push_back(anim);
				}
			}

			fclose(file);
		}
	};
}