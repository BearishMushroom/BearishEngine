#ifndef _BEARISH_GRAPHICS_VERTEX_H_
#define _BEARISH_GRAPHICS_VERTEX_H_

#include "../Math/vec2.h"
#include "../Math/vec3.h"
#include "../Math/vec4.h"

namespace Bearish { namespace Graphics { 
	class Vertex {
	public:
		Math::vec3 position; 
		Math::vec2 texCoord;
		Math::vec3 normal;
		Math::vec3 tangent;
		Math::vec4i boneIDs;
		Math::vec4 boneWeights;

		Vertex(Math::vec3 position = Math::vec3(0),
				Math::vec2 texCoord = Math::vec2(0),
				Math::vec3 normal = Math::vec3(0),
				Math::vec3 tangent = Math::vec3(0),
				Math::vec4i boneIDs = Math::vec4i(-1, -1, -1, -1),
				Math::vec4 boneWeights = Math::vec4(0, 0, 0, 0)) : 
		position(position), texCoord(texCoord), normal(normal), tangent(tangent), boneIDs(boneIDs), boneWeights(boneWeights) {}

		void AddBoneID(i32 id, f32 weight) {
			for (i32 i = 0; i < 4; i++) {
				if (boneIDs[i] == -1) {
					boneIDs[i] = id;
					boneWeights[i] = weight;
					return;
				}
			}
		}

		f32* ToArray() const {
			f32* result = new f32[sizeof(Vertex) / sizeof(f32)];
			result[0] = position.x;
			result[1] = position.y;
			result[2] = position.z;

			result[3] = texCoord.x;
			result[4] = texCoord.y;

			result[5] = normal.x;
			result[6] = normal.y;
			result[7] = normal.z;

			result[8] = tangent.x;
			result[9] = tangent.y;
			result[10] = tangent.z;
		}
	};
}}

#endif