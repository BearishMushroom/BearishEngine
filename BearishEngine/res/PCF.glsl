uniform int shadowMS;

const vec2 disk[17] = vec2[] (
  vec2(0.04213448, -0.5144756),
  vec2(-0.7202017, -0.2952146),
  vec2(0.7221854, -0.2234438),
  vec2(-0.06546286, -0.06506993),
  vec2(0.1782186, -0.9195618),
  vec2(-0.5211688, -0.7380918),
  vec2(-0.4480472, 0.05305893),
  vec2(0.9554358, 0.2335221),
  vec2(0.390198, 0.4263327),
  vec2(0.5188487, -0.6759724),
  vec2(-0.4816897, 0.7580756),
  vec2(-0.6916474, 0.3981213),
  vec2(-0.1279221, 0.4786578),
  vec2(-0.9854926, 0.03731598),
  vec2(0.5436422, 0.8070505),
  vec2(0.3282386, -0.1488035),
  vec2(0.1374841, 0.8604584)
);

float linstep(float l, float h, float v) {
  return clamp((v-l)/(h-l), 0.0, 1.0);
}

float CalculateShadow(sampler2D sm, vec4 lightPos, float ndl) {
  if(shadowMS == 0) return 1.0;

  //if(ndl < 0) return 0.0;
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
/*
  vec2 moments = texture(sm, li.xy).rg;
  float p = step(li.z, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.00002);
  float d = li.z - moments.x;
  float pMax = linstep(0.2, 1.0, variance / (variance + d * d));

  return min(max(p, pMax), 1.0);*/

  if(ndl < 0) {
    return 0.0;
  }

  float shadow = 0;
  float bias = 0.005*tan(acos(ndl)); // cosTheta is dot( n,l ), clamped between 0 and 1
  bias = clamp(bias, 0, 0.01);

  if(li.x < 0.001 || li.x > 0.999 || li.y < 0.001 || li.y > 0.999) return 1.0;

  for(int i = 0; i < 15; i++) {
    shadow += (texture(sm, li.xy + (disk[i] / 1400)).r > li.z - bias) ? 1 : 0;
  }

  return shadow / 15.0;
}
