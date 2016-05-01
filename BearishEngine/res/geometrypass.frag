#version 330

#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

in vec3 worldPos0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 tangent0;

layout (location = 0) out vec3 Out_World;
layout (location = 1) out vec2 Out_Normal;
layout (location = 2) out vec3 Out_Diffuse;
layout (location = 3) out vec4 Out_SpecRoughness;

uniform sampler2D AlbedoMap;
uniform sampler2D SpecularMap;
uniform sampler2D GlossMap;
uniform sampler2D NormalMap;

	// PBR Static Inputs
uniform vec4 AlbedoColor;
uniform vec3 SpecularColor;
uniform float GlossColor;

	// PBR Modes
uniform float UsingAlbedoMap;
uniform float UsingSpecularMap;
uniform float UsingGlossMap;

vec4 GetAlbedo(vec2 uv) {
	return (1.0 - UsingAlbedoMap) * AlbedoColor + UsingAlbedoMap * texture(AlbedoMap, uv).rgba;
}

vec3 GetSpecular(vec2 uv) {
	return (1.0 - UsingSpecularMap) * SpecularColor + UsingSpecularMap * texture(SpecularMap, uv).rgb;
}

float GetGloss(vec2 uv) {
	return (1.0 - UsingGlossMap) * GlossColor + UsingGlossMap * texture(GlossMap, uv).r;
}

float GetRoughness(vec2 uv) {
	return 1.0 - GetGloss(uv);

}

#include "res/LAEA.glsl"

void main() {
  Out_World = worldPos0;

  // Calculate normalmap
  vec3 tangent = normalize(tangent0 - dot(tangent0, normal0) * normal0);
  vec3 biTangent = cross(tangent, normal0);
  mat3 tbn = mat3(tangent, biTangent, normal0);

  vec3 normal = normalize(tbn * (2 * texture(NormalMap, texCoord0).xyz - 1));

  Out_Normal = EncodeNormal(normal);
  vec4 albedo = GetAlbedo(texCoord0);
  Out_Diffuse = albedo.rgb;
  Out_SpecRoughness = vec4(GetSpecular(texCoord0), GetRoughness(texCoord0));
}
