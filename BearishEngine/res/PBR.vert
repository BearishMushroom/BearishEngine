#version 330

layout (location = 0) in vec3 position;

uniform	mat4 world;
uniform	mat4 MVP;

uniform mat4 lightMVP[3];

out mat4 lightMat[3];

const mat4 lightBias = mat4(
vec4(0.5, 0,   0,   0),
vec4(0,   0.5, 0,   0),
vec4(0,   0,   0.5, 0),
vec4(0.5, 0.5, 0.5, 1.0));
 
void main() {
	gl_Position = MVP * vec4(position, 1);

	lightMat[0] = lightBias * lightMVP[0];

	lightMat[1] = lightBias * lightMVP[1];

	lightMat[2] = lightBias * lightMVP[2];
	
}
