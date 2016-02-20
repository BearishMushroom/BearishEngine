#version 330 core

#include "res/LightStructs.glsl"

out vec4 fragcolor;

const int LIGHT_DIRECTIONAL = 0;
const int LIGHT_POINT = 1;
const int LIGHT_SPOT = 2;

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
uniform float specularIntensity;
uniform float specularPower;
uniform int matID;

uniform sampler2D diffuse;
uniform sampler2D normalMap;

#include "res/LightCalcs.glsl"

in mat4 lightMat;

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

		n = normalize(tbn * (255.0/128.0 * texture(normalMap, texCoord).xyz - 1));
		vec4 diff = texture(diffuse, texCoord);

		if (light == LIGHT_DIRECTIONAL) {
			vec4 lp = (lightMat * vec4(w, 1));
			fragcolor = diff * CalculateLight(dLight.base, -dLight.direction, n, w)
				* CalculateShadow(shadowMap, lp);
		}

		if (light == LIGHT_POINT) {
			fragcolor = diff * vec4(pLight.base.color, 1.0) * CalculatePointLight(pLight, n, w);
		}

		if (light == LIGHT_SPOT) {
			fragcolor = diff * vec4(spotLight.base.base.color, 1.0) * CalculateSpotLight(spotLight, n, w);
		}
	} else {
		discard;
	}
}
