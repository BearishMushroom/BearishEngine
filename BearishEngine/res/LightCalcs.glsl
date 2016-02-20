const vec2 poissonDisk[4] = vec2[](
  vec2(-0.94201624, -0.39906216),
  vec2( 0.94558609, -0.76890725),
  vec2(-0.094184101 -0.92938870),
  vec2( 0.34495938,  0.29387760)
);

const mat4 lightBias = mat4(
vec4(0.5, 0,   0,   0),
vec4(0,   0.5, 0,   0),
vec4(0,   0,   0.5, 0),
vec4(0.5, 0.5, 0.5, 1.0));

vec4 CalculateLight(Light light, vec3 direction, vec3 normal, vec3 wp) {
	vec4 ambientColor = vec4(light.color * light.ambientIntensity, 1.0);
	vec3 lightDirection = -direction;

	float diffuseFactor = dot(normal, lightDirection);

	vec4 diffuseColor = vec4(0);
	vec4 specularColor = vec4(0);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;

		vec3 vertexToEye = normalize(eyePos - wp);
		vec3 lightReflect = normalize(reflect(direction, normal));
		float specularFactor = dot(vertexToEye, lightReflect);

		if (specularFactor > 0) {
			specularFactor = pow(specularFactor, specularPower);
			specularColor = vec4(light.color * specularIntensity * specularFactor, 1.0);
		}
	}

	return vec4((ambientColor + diffuseColor + specularColor).rgb, 1);
}

vec4 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 wp) {
	vec3 lightDirection = wp - pointLight.position;
	float distanceToPoint = length(lightDirection);

	lightDirection = normalize(lightDirection);

	vec4 color = CalculateLight(pointLight.base, lightDirection, normal, wp);

	float attenuation = pointLight.attenuation.constant +
						pointLight.attenuation.linear * distanceToPoint +
						pointLight.attenuation.exponent * distanceToPoint * distanceToPoint +
						0.0001;

	return color / attenuation;
}

vec4 CalculateSpotLight(SpotLight light, vec3 normal, vec3 wp) {
    vec3 lightToPixel = normalize(wp - light.base.position);
    float spotFactor = dot(lightToPixel, -light.direction);

    if (spotFactor > light.cutoff) {
        vec4 color = CalculatePointLight(light.base, normal, wp);
        return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - light.cutoff));
    }
    else {
        return vec4(0);
    }
}

float CalculateShadow(sampler2D sm, vec4 lightPos) {
	float result = 1;
	vec4 coords = lightPos / lightPos.w;
	for (int i = 0; i < 4; i++){
		if (texture(sm, coords.xy + poissonDisk[i] / 700.0).r < coords.z - 0.03) {
			result -= 0.2;
		}
	}

	return result;
}

vec2 CalcTexCoord(vec2 screenSize) {
  return gl_FragCoord.xy / screenSize;
}
