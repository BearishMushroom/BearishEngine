uniform int pcfSize;
uniform int shadowSize;

float CalculateShadow(sampler2D sm, vec4 lightPos) {
  if(shadowSize > 0) {
    float texelCount = (pcfSize * 2.0 + 1.0) * (pcfSize * 2.0 + 1.0);
  	vec4 coords = lightPos / lightPos.w;
    float texelSize = 1.0 / shadowSize;

    float texels = 0;
    for(int x = -pcfSize; x <= pcfSize; x++) {
      for(int y = -pcfSize; y <= pcfSize; y++) {
        vec2 coords2 = coords.xy + vec2(x, y) * texelSize;
        if(coords2.x < 0.001 || coords2.y < 0.001 || coords2.x > 0.999 || coords2.y > 0.999) {
            continue;
        }

        if(texture(sm, coords2).r < coords.z - 0.001) {
          texels++;
        }
      }
    }

    float result = 1 - texels / texelCount;

    return result;
  } else {
    return 1.0;
  }
}
