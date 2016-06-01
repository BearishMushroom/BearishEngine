uniform int pcfSize;
uniform int shadowSize;

float CalculateShadow(sampler2DShadow sm, vec4 lightPos, float ndotl) {
  float texelCount = (pcfSize * 2.0 + 1.0) * (pcfSize * 2.0 + 1.0);

  float texels = 0;
  for(float x = -1.5; x <= 1.5; x += 1.0) {
    for(float y = -1.5; y <= 1.5; y += 1.0) {
      texels += textureProj(sm, vec4(lightPos.xy + vec2(x, y) *
                                (1.0/float(shadowSize)) * lightPos.w, lightPos.z - 0.01, lightPos.w));
    }
  }

  return texels / 16;
}
