#version 330

in vec3 texCoord0;

out vec4 FragColor;

uniform samplerCube diffuse;

void main() {
    FragColor = texture(diffuse, texCoord0);
}