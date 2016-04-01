vec3 RadianceIBLIntegration(sampler2D fg, float NdotV, float roughness, vec3 specular) {
	vec2 preintegratedFG = texture(fg, vec2(roughness, 1.0 - NdotV)).rg;
	return specular * preintegratedFG.r + preintegratedFG.g;
}

vec3 IBL(sampler2D fg, samplerCube envionment, vec4 albedo, vec3 specular, float roughness, vec3 normal, vec3 eye) {
	float NdotV = max(dot(normal, eye), 0.0);

	vec3 reflectionVector = normalize(reflect(-eye, normal));
	float smoothness = 1.0f - roughness;
	float mipLevel = (1.0 - smoothness * smoothness) * 10.0;
	vec4 cs = textureLod(envionment, reflectionVector, mipLevel);
	vec3 result = pow(cs.xyz, vec3(GAMMA)) * RadianceIBLIntegration(fg, NdotV, roughness, specular);

	vec3 diffuseDominantDirection = normal;
	float diffuseLowMip = 9.6;
	vec3 diffuseImageLighting = textureLod(envionment, diffuseDominantDirection, diffuseLowMip).rgb;
	diffuseImageLighting = pow(diffuseImageLighting, vec3(GAMMA));

	return result + diffuseImageLighting * albedo.rgb;
}
