uniform int pcfSize;
uniform int shadowSize;

float CalculateShadow(sampler2D sm, vec4 lightPos) {
  if(shadowSize > 0) {
    float texelCount = (pcfSize * 2.0 + 1.0) * (pcfSize * 2.0 + 1.0);
  	vec4 coords = lightPos / lightPos.w;

    float texels = 0;
    for(int x = -pcfSize; x <= pcfSize; x++) {
      for(int y = -pcfSize; y <= pcfSize; y++) {
        vec2 coords2 = clamp(coords.xy + vec2(x, y) * (1.0 / shadowSize), 0.001, 0.999);

        if(texture(sm, coords2).r < coords.z - 0.001) {
          texels++;
        }
      }
    }

    return 1 - texels / texelCount;
  } else {
    return 1.0;
  }
}
