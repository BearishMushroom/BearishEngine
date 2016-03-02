#version 330

out vec4 fragColor;

in vec2 texCoord0;

uniform sampler2D diffuse;

void main() {
  vec4 color = texture(diffuse, texCoord0);

  if(length(color.xyz) < 0.03) {
    discard;
  }

  if(color.a < 0.05) {
    discard;
  }

  fragColor = color;
}
