vec3 GGX(vec3 direction, vec3 normal, vec3 specular, float roughness, vec3 eye) {
	vec3 h = normalize(direction + eye);
	float NdotH = max(dot(normal, h), 0.0);

	float rough2 = max(roughness * roughness, 2.0e-3); // capped so spec highlights don't disappear
	float rough4 = rough2 * rough2;

	float d = (NdotH * rough4 - NdotH) * NdotH + 1.0;
	float D = rough4 / (PI * (d * d));

	// Fresnel
	vec3 reflectivity = specular;
	float fresnel = 1.0;
	float NdotL = clamp(dot(normal, direction), 0.0, 1.0);
	float LdotH = clamp(dot(direction, h), 0.0, 1.0);
	float NdotV = clamp(dot(normal, eye), 0.0, 1.0);
	vec3 F = reflectivity + (fresnel - fresnel * reflectivity) * exp2((-5.55473 * LdotH - 6.98316) * LdotH);

	// geometric / visibility
	float k = rough2 * 0.5;
	float G_SmithL = NdotL * (1.0 - k) + k;
	float G_SmithV = NdotV * (1.0 - k) + k;
	float G = 0.25 / (G_SmithL * G_SmithV);

	return G * D * F;
}
