vec3 RadianceIBLIntegration(sampler2D fg, float NdotV, float roughness, vec3 specular) {
	vec2 preintegratedFG = texture(fg, vec2(roughness, 1.0 - NdotV)).rg;
	return specular * preintegratedFG.r + preintegratedFG.g;
}

vec3 IBL(sampler2D fg, samplerCube envionment, vec4 albedo, vec3 specular, float roughness, vec3 normal, vec3 eye) {
	float smoothness = 1.0f - roughness;
	vec3 result = pow(textureLod(envionment, normalize(reflect(-eye, normal)), (1.0 - smoothness * smoothness) * 10.0).xyz,
										vec3(GAMMA)) * RadianceIBLIntegration(fg, max(dot(normal, eye), 0.0), roughness, specular);

	vec3 diffuseImageLighting = pow(textureLod(envionment, normal, 9.6).rgb, vec3(GAMMA));

	return result + diffuseImageLighting * albedo.rgb;
}
