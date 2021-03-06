#version 330 core
#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

#include "res/LightStructs.glsl"

out vec4 fragcolor;

const int LIGHT_DIRECTIONAL = 0;
const int LIGHT_POINT = 1;

// Engine uniforms.
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecRoughness;

uniform sampler2D shadowMap[3];

uniform float shadowBounds[3];

in mat4 lightMat[3];

uniform DirectionalLight dLight = { { {0, 0, 0}, 0, 0}, {0, 0, 0}};
uniform PointLight pLight = { { {0, 0, 0}, 0, 0}, {0, 0, 0}, {0, 0, 0}};
uniform int light;

uniform vec2 screen;
uniform vec3 eyePos;

// Material uniforms.
// PBR Inputs

uniform sampler2D PreintegratedFG;
uniform samplerCube EnvironmentMap;

uniform sampler2D gSSAO;
uniform float ssaoScale;

vec4 GammaCorrectTexture(sampler2D tex, vec2 uv) {
	vec4 samp = texture(tex, uv);
	return vec4(pow(samp.rgb, vec3(GAMMA)), samp.a);
}

vec3 GammaCorrectTextureRGB(sampler2D tex, vec2 uv) {
	vec4 samp = texture(tex, uv);
	return vec3(pow(samp.rgb, vec3(GAMMA)));
}

vec3 FinalGamma(vec3 color) {
	return pow(color, vec3(1.0 / GAMMA));
}

vec2 CalcTexCoord(vec2 screenSize) {
  return gl_FragCoord.xy / screenSize;
}

#include "res/Disney.glsl"
#include "res/GGX.glsl"
#include "res/IBL.glsl"
#include "res/PCF.glsl"
#include "res/LAEA.glsl"

vec3 PBR(float visibility, vec4 albedo, vec3 specular, float roughness, vec3 world, vec3 normal, vec3 eye, vec3 direction, vec3 color, float intensity) {
  float NdotL = clamp(dot(normal, direction), 0.0, 1.0);

  vec4 diffuse = NdotL * Disney(direction, roughness, normal, eye) * vec4(color, 1.0) * intensity;
  vec3 spec = NdotL * GGX(direction, normal, specular, roughness, eye) * color * intensity;

  vec3 finalColor = albedo.rgb * diffuse.rgb * visibility +
                    (spec + IBL(PreintegratedFG, EnvironmentMap, albedo, specular, roughness, normal, eye)) * visibility;
  finalColor = FinalGamma(finalColor);
  return finalColor;
}

void main() {
	vec2 tc = CalcTexCoord(screen);

	vec3 position = texture(gPosition, tc).xyz;
	vec3 normal = (texture(gNormal, tc).xyz);
	vec3 eye = normalize(eyePos - position);

	vec4 albedo = vec4(GammaCorrectTextureRGB(gDiffuse, tc), 1);

	vec4 specRough = GammaCorrectTexture(gSpecRoughness, tc);
	vec3 specular = specRough.rgb;
	float roughness = specRough.a;

	if (light == LIGHT_DIRECTIONAL) {
		float NdotL = dot(normal, dLight.direction);

	  float visibility = 1;
		float dist = texture(gPosition, tc).a;

		if(dist <= shadowBounds[0]) {
			visibility = CalculateShadow(shadowMap[0], lightMat[0] * vec4(position, 1), NdotL);
		} else if(dist <= shadowBounds[1]) {
			visibility = CalculateShadow(shadowMap[1], lightMat[1] * vec4(position, 1), NdotL);
		} else if(dist <= shadowBounds[2]) {
			visibility = CalculateShadow(shadowMap[2], lightMat[2] * vec4(position, 1), NdotL);
		}

		vec4 final = vec4(PBR(visibility, albedo, specular, roughness, position, normal, eye, dLight.direction, dLight.base.color, dLight.base.diffuseIntensity), albedo.a);
		final += ssaoScale > 0 ? albedo * dLight.base.ambientIntensity * texture(gSSAO, tc).r : vec4(0);
	  fragcolor = final;// + vec4(ttint, 0);
	}

	if (light == LIGHT_POINT) {
		vec3 lightDirection = position - pLight.position;
		float distanceToPoint = length(lightDirection);

		lightDirection = normalize(lightDirection);

		vec4 color = vec4(PBR(1.0, albedo, specular, roughness, position, normal, eye, lightDirection, pLight.base.color, pLight.base.diffuseIntensity), albedo.a);

		float attenuation = 1 / (1 + pLight.attenuation.constant +
  						pLight.attenuation.linear * distanceToPoint +
  						pLight.attenuation.exponent * distanceToPoint * distanceToPoint);

		vec4 final = color * attenuation;
		final += ssaoScale > 0 ? albedo * pLight.base.ambientIntensity * texture(gSSAO, tc).r : vec4(0);
		fragcolor = final;
	}
}
