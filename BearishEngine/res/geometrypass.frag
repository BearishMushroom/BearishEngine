#version 330

in vec3 worldPos0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 tangent0;

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 TangentOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 TexCoordOut;

uniform int matID;

void main() {
  TexCoordOut = vec3(texCoord0, matID);
  WorldPosOut = worldPos0;
  TangentOut = tangent0;
  NormalOut = normal0;
}
