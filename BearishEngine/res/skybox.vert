#version 330

layout (location = 0) in vec3 position;

uniform	mat4 world;
uniform	mat4 MVP;

out vec3 texCoord0;

void main() {
    vec4 pos = MVP * vec4(position, 1.0);
    gl_Position = pos.xyww;
    texCoord0 = normalize(position);
}
