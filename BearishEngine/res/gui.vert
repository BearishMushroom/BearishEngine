#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 10) in mat4 MVP;

out vec2 texCoord0;

void main() {
    gl_Position = MVP * vec4(position, 1);
    texCoord0 = texCoord;
}
