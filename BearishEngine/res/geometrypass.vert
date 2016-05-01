#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in ivec4 boneIDs;
layout (location = 5) in vec4 boneWeights;

uniform	mat4 world;
uniform	mat4 MVP;

out vec3 worldPos0;
out vec2 texCoord0;
out vec3 normal0;
out vec3 tangent0;

uniform mat4 bones[100];

void main() {
	mat4 boneTransform = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	if(boneIDs[0] != -1) {
		boneTransform = bones[boneIDs[0]] * boneWeights[0];
		boneTransform += bones[boneIDs[1]] * boneWeights[1];
		boneTransform += bones[boneIDs[2]] * boneWeights[2];
		boneTransform += bones[boneIDs[3]] * boneWeights[3];
	}

	vec4 bonePos = boneTransform * vec4(position, 1);
	vec4 boneNormal = boneTransform * vec4(normal, 0);
	vec4 boneTang = boneTransform * vec4(tangent, 0);

	gl_Position = MVP * bonePos;
	worldPos0 = (world * bonePos).xyz;
	texCoord0 = texCoord;
	normal0 = normalize((world * boneNormal).xyz);

	tangent0 = normalize((world * boneTang).xyz);
}
