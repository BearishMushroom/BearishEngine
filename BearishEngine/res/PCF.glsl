uniform int shadowMS;

float linstep(float l, float h, float v) {
  return clamp((v-l)/(h-l), 0.0, 1.0);
}

float CalculateShadow(sampler2D sm, vec4 lightPos, float ndl) {
  if(ndl < 0) return 0;
  vec3 li = lightPos.xyz / lightPos.w;
  /*float texelCount = (pcfSize * 2.0 + 1.0) * (pcfSize * 2.0 + 1.0);

  float texels = 0;
  for(float x = -1.5; x <= 1.5; x += 1.0) {
    for(float y = -1.5; y <= 1.5; y += 1.0) {
      texels += LinearShadow(sm, li.xy + vec2(x, y) * texelSize, li.z - 0.01);
    }
  }

  return texels / 16;*/
  /*ivec2 size = textureSize(sm);

  vec2 val = vec2(0.0);
  for(int i = 0; i < shadowMS; i++) {
    val += texelFetch(sm, ivec2(li.xy) * size, i).rg;
  }

  vec2 moments = val / vec2(shadowMS);
  */

  vec2 moments = texture(sm, li.xy).rg;
  float p = step(li.z, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.00002);
  float d = li.z - moments.x;
  float pMax = linstep(0.2, 1.0, variance / (variance + d * d));

  return min(max(p, pMax), 1.0);
}
