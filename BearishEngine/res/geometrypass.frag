#version 330

#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

in vec3 worldPos0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 tangent0;

layout (location = 0) out vec4 Out_World;
layout (location = 1) out vec3 Out_Normal;
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
uniform float UsingAlbedoMap = 0;
uniform float UsingSpecularMap = 0;
uniform float UsingGlossMap = 0;
uniform float UsingNormalMap = 0;

uniform vec2 CameraPlanes;

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

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * CameraPlanes.x * CameraPlanes.y) / (CameraPlanes.y + CameraPlanes.x - z * (CameraPlanes.y - CameraPlanes.x));
}

void main() {
	vec4 albedo = GetAlbedo(texCoord0);
	if(albedo.a < 0.9) discard;

  Out_World = vec4(worldPos0, LinearizeDepth(gl_FragCoord.z));

  // Calculate normalmap
  vec3 biTangent = cross(tangent0, normal0);
  mat3 tbn = mat3(tangent0, biTangent, normal0);

  vec3 normal = (1 - UsingNormalMap) * normal0 + UsingNormalMap * normalize(tbn * (2 * texture(NormalMap, texCoord0).xyz - 1));

  Out_Normal = normal;
  Out_Diffuse = albedo.rgb;
  Out_SpecRoughness = vec4(GetSpecular(texCoord0), GetRoughness(texCoord0));
}
