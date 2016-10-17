#ifndef _BEARISH_GRAPHICS_VERTEX_H_
#define _BEARISH_GRAPHICS_VERTEX_H_

#include <BE/Math/vec2.h>
#include <BE/Math/vec3.h>
#include <BE/Math/vec4.h>

namespace Bearish { namespace Graphics { 
	class BEARISH_API Vertex {
	public:
		Math::vec3 position;
		Math::vec2 texCoord;
		Math::vec3 normal;
		Math::vec3 tangent;

		Vertex(Math::vec3 position = Math::vec3(0),
			Math::vec2 texCoord = Math::vec2(0),
			Math::vec3 normal = Math::vec3(0),
			Math::vec3 tangent = Math::vec3(0)) :
			position(position), texCoord(texCoord), normal(normal), tangent(tangent) {}
	};

	class BEARISH_API SkinnedVertex {
	public:
		Math::vec3 position;
		Math::vec2 texCoord;
		Math::vec3 normal;
		Math::vec3 tangent;
		Math::vec4i boneIDs;
		Math::vec4 boneWeights;

		SkinnedVertex(Math::vec3 position = Math::vec3(0),
			Math::vec2 texCoord = Math::vec2(0),
			Math::vec3 normal = Math::vec3(0),
			Math::vec3 tangent = Math::vec3(0),
			Math::vec4i boneIDs = Math::vec4i(-1, -1, -1, -1),
			const Math::vec4& boneWeights = Math::vec4(0, 0, 0, 0)) :
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
	};
}}

#endif