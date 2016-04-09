float FresnelSchlick(float f0, float fd90, float view) {
	return f0 + (fd90 - f0) * pow(max(1.0 - view, 0.1), 5.0);
}

float Disney(vec3 direction, float roughness, vec3 normal, vec3 eye) {
	float LdotH = max(dot(direction, normalize(direction + eye)), 0.0);

	float energyFactor = mix(1.0, 1.0 / 1.51, roughness);
	float fd90 = mix(0.0, 0.5, roughness) + 2.0 * (LdotH * LdotH) * roughness;

	float lightScatter = FresnelSchlick(1.0, fd90,  max(dot(normal, direction), 0.0));
	float viewScatter = FresnelSchlick(1.0, fd90, max(dot(normal, eye), 0.0));

	return lightScatter * viewScatter * energyFactor;
}
