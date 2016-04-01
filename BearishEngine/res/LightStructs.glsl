struct Light {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight {
	Light base;
	vec3 direction;
};

struct Attenuation {
	float constant;
	float linear;
	float exponent;
};

struct PointLight {
	Light base;
	Attenuation attenuation;
	vec3 position;
};

struct SpotLight {
	PointLight base;
	vec3 direction;
	float cutoff;
};
