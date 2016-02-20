#version 330

#include "res/LightStructs.glsl"

out vec4 fragcolor;

in vec3 worldPos0;
in vec2 texCoord0;
in vec3 normal0;
in mat3 tbn;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

uniform SpotLight spotLight = {0};
uniform PointLight pointLight = {0};

uniform vec3 eyePos;
uniform float specularIntensity;
uniform float specularPower;

#include "res/LightCalcs.glsl"

void main() {
	vec3 normal = texture(normalMap, texCoord0).xyz;
	normal = 2 * normal - vec3(1);
	normal = normalize(normal * tbn);
	fragcolor = texture2D(diffuse, texCoord0);
	if (length(pointLight.color) > 0.01) {
		fragcolor *= CalculatePointLight(light, normalize(normal0));
	}

	if (length(spotLight.color) > 0.01) {
		fragcolor *= CalculateSpotLight(light, normalize(normal0));
	}
}
