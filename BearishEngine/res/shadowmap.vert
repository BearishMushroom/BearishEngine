#version 330

layout (location = 0) in vec3 position;
layout (location = 4) in ivec4 boneIDs;
layout (location = 5) in vec4 boneWeights;

layout(std140) uniform instance_data {
	mat4 world;
	mat4 MVP;
	float rigged;
};

uniform mat4 bones[100];

void main() {
	mat4 boneTransform = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	if(rigged > 0) {
			boneTransform = bones[boneIDs[0]] * boneWeights[0];
			boneTransform += bones[boneIDs[1]] * boneWeights[1];
			boneTransform += bones[boneIDs[2]] * boneWeights[2];
			boneTransform += bones[boneIDs[3]] * boneWeights[3];
	}

	vec4 bonePos = boneTransform * vec4(position, 1);
	gl_Position = MVP * bonePos;
}
