#version 330

uniform sampler2D diffuse;

in vec2 texCoord1;
in vec4 color1;

out vec4 fragColor;

void main() {
  fragColor = texture2D(diffuse, texCoord1) * color1;
}
