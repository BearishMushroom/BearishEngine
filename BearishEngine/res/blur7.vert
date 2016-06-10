#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 texCoords0[7];

uniform	mat4 MVP;
uniform vec2 scale = vec2(0, 0);

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    texCoords0[0] = texCoord + scale * -0.012;
    texCoords0[1] = texCoord + scale * -0.008;
    texCoords0[2] = texCoord + scale * -0.004;
    texCoords0[3] = texCoord + scale;
    texCoords0[4] = texCoord + scale *  0.004;
    texCoords0[5] = texCoord + scale *  0.008;
    texCoords0[6] = texCoord + scale *  0.012;
}
