#version 330 core
#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

#include "res/LightStructs.glsl"

out vec4 fragcolor;

const int LIGHT_DIRECTIONAL = 0;
const int LIGHT_POINT = 1;
const int LIGHT_SPOT = 2;

// Engine uniforms.
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecRoughness;

uniform sampler2D shadowMap;

uniform DirectionalLight dLight = { { {0, 0, 0}, 0, 0}, {0, 0, 0}};
uniform PointLight pLight = { { {0, 0, 0}, 0, 0}, {0, 0, 0}, {0, 0, 0}};
uniform SpotLight spotLight = { { { {0, 0, 0}, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {0, 0, 0}, 0};
uniform int light;

uniform vec2 screen;
uniform vec3 eyePos;

in mat4 lightMat;

// Material uniforms.
// PBR Inputs
uniform sampler2D PreintegratedFG;
uniform samplerCube EnvironmentMap;

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
	vec3 normal = DecodeNormal(texture(gNormal, tc).xy);
  vec3 eye = normalize(eyePos - position);

  vec4 albedo = vec4(GammaCorrectTextureRGB(gDiffuse, tc), 1);

	vec4 specRough = GammaCorrectTexture(gSpecRoughness, tc);
  vec3 specular = specRough.rgb;
  float roughness = specRough.a;

	if (light == LIGHT_DIRECTIONAL) {
	  vec4 lp = (lightMat * vec4(position, 1));
	  float visibility = CalculateShadow(shadowMap, lp);
	  vec4 final = vec4(PBR(visibility, albedo, specular, roughness, position, normal, eye, dLight.direction, dLight.base.color, dLight.base.diffuseIntensity), albedo.a);
	  fragcolor = final;
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
		fragcolor = final;
	}

	if (light == LIGHT_SPOT) {
		//fragcolor = diff * vec4(spotLight.base.base.color, 1.0) * CalculateSpotLight(spotLight, n, w);
    discard;
	}
}
