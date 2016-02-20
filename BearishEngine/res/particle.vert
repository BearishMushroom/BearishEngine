#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 scale;
layout (location = 2) in float rotation;
layout (location = 3) in vec4 color;

out vec3 position0;
out vec3 scale0;
out float rotation0;
out vec4 color0;

void main() {
  gl_Position = vec4(position, 1.0);
  position0 = position;
  scale0 = scale;
  rotation0 = rotation;
  color0 = color;
}
