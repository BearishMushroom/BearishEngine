float FresnelSchlick(float f0, float fd90, float view) {
	return f0 + (fd90 - f0) * pow(max(1.0 - view, 0.1), 5.0);
}

float Disney(vec3 direction, float roughness, vec3 normal, vec3 eye) {
	vec3 halfVector = normalize(direction + eye);

	float NdotL = max(dot(normal, direction), 0.0);
	float LdotH = max(dot(direction, halfVector), 0.0);
	float NdotV = max(dot(normal, eye), 0.0);

	float energyBias = mix(0.0, 0.5, roughness);
	float energyFactor = mix(1.0, 1.0 / 1.51, roughness);
	float fd90 = energyBias + 2.0 * (LdotH * LdotH) * roughness;
	float f0 = 1.0;

	float lightScatter = FresnelSchlick(f0, fd90, NdotL);
	float viewScatter = FresnelSchlick(f0, fd90, NdotV);

	return lightScatter * viewScatter * energyFactor;
}
