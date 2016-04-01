#version 330 core
#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

#include "res/LightStructs.glsl"

out vec4 fragcolor;

const int LIGHT_DIRECTIONAL = 0;
const int LIGHT_POINT = 1;
const int LIGHT_SPOT = 2;

// Engine uniforms.
uniform sampler2D gWorld;
uniform sampler2D gTangent;
uniform sampler2D gNormal;
uniform sampler2D gTexCoord;

uniform sampler2D shadowMap;

uniform DirectionalLight dLight = { { {0, 0, 0}, 0, 0}, {0, 0, 0}};
uniform PointLight pLight = { { {0, 0, 0}, 0, 0}, {0, 0, 0}, {0, 0, 0}};
uniform SpotLight spotLight = { { { {0, 0, 0}, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {0, 0, 0}, 0};
uniform int light;

uniform vec2 screen;
uniform vec3 eyePos;
uniform int matID;

in mat4 lightMat;

// Material uniforms.
// PBR Inputs
uniform sampler2D PreintegratedFG;
uniform samplerCube EnvironmentMap;

// PBR Map Inputs
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

vec4 GammaCorrectTexture(sampler2D tex, vec2 uv) {
	vec4 samp = texture(tex, uv);
	return vec4(pow(samp.rgb, vec3(GAMMA)), samp.a);
}

vec3 GammaCorrectTextureRGB(sampler2D tex, vec2 uv) {
	vec4 samp = texture(tex, uv);
	return vec3(pow(samp.rgb, vec3(GAMMA)));
}

vec4 GetAlbedo(vec2 uv) {
	return (1.0 - UsingAlbedoMap) * AlbedoColor + UsingAlbedoMap * GammaCorrectTexture(AlbedoMap, uv);
}

vec3 GetSpecular(vec2 uv) {
	return (1.0 - UsingSpecularMap) * SpecularColor + UsingSpecularMap * GammaCorrectTextureRGB(SpecularMap, uv);
}

float GetGloss(vec2 uv) {
	return (1.0 - UsingGlossMap) * GlossColor + UsingGlossMap * GammaCorrectTextureRGB(GlossMap, uv).r;
}

float GetRoughness(vec2 uv) {
	return 1.0 - GetGloss(uv);
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

vec3 PBR(vec4 albedo, vec3 specular, float roughness, vec3 world, vec3 normal, vec3 eye, vec3 direction, vec3 color, float intensity) {
  float NdotL = clamp(dot(normal, direction), 0.0, 1.0);

  vec4 diffuse = NdotL * Disney(direction, roughness, normal, eye) * vec4(color, 1.0) * intensity;
  vec3 spec = NdotL * GGX(direction, normal, specular, roughness, eye) * color * intensity;

  vec4 lp = (lightMat * vec4(world, 1));
  float visibility = CalculateShadow(shadowMap, lp);

  vec3 finalColor = albedo.rgb * diffuse.rgb * visibility +
                    (spec + IBL(PreintegratedFG, EnvironmentMap, albedo, specular, roughness, normal, eye)) * visibility;
  finalColor = FinalGamma(finalColor);
  return finalColor;
}

void main() {
	vec2 tc = CalcTexCoord(screen);
	int mat = int(texture(gTexCoord, tc).z);

	if (mat == matID) {
		vec3 n = texture(gNormal, tc).xyz;
		vec3 w = texture(gWorld, tc).xyz;
		vec3 t = texture(gTangent, tc).xyz;
		vec2 texCoord = texture(gTexCoord, tc).xy;

		vec3 tangent = normalize(t - dot(t, n) * n);
		vec3 biTangent = cross(tangent, n);
		mat3 tbn = mat3(tangent, biTangent, n);

		n = normalize(tbn * (255.0/128.0 * texture(NormalMap, texCoord).xyz - 1));

    vec3 eye = normalize(eyePos - w);

    vec4 albedo = GetAlbedo(texCoord);
    vec3 specular = GetSpecular(texCoord);
    float roughness = GetRoughness(texCoord);

		if (light == LIGHT_DIRECTIONAL) {
      fragcolor = vec4(PBR(albedo, specular, roughness, w, n, eye, dLight.direction, dLight.base.color, dLight.base.diffuseIntensity), albedo.a);
		}

		if (light == LIGHT_POINT) {
    	vec3 lightDirection = w - pLight.position;
    	float distanceToPoint = length(lightDirection);

    	lightDirection = normalize(lightDirection);

    	vec4 color = vec4(PBR(albedo, specular, roughness, w, n, eye, lightDirection, pLight.base.color, pLight.base.diffuseIntensity), albedo.a);

    	float attenuation = pLight.attenuation.constant +
    						pLight.attenuation.linear * distanceToPoint +
    						pLight.attenuation.exponent * distanceToPoint * distanceToPoint +
    						0.0001;

    	fragcolor = color / attenuation;
    }

		if (light == LIGHT_SPOT) {
			//fragcolor = diff * vec4(spotLight.base.base.color, 1.0) * CalculateSpotLight(spotLight, n, w);
      discard;

		}
	} else {
		discard;
	}
}
