#version 330

out float ssaoFactor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform sampler2D Noise;
uniform vec2 Screen;
uniform mat4 Projection;

const float g_scale = 0.3;
const float g_intensity = 3;
const float g_bias = 0.3;
const float g_sample_rad = 1;

float doAmbientOcclusion(vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm) {
  vec3 diff = texture(gPosition, tcoord + uv).xyz - p;
  vec3 v = normalize(diff);
  float d = length(diff) * g_scale;
  return max(0.0, dot(cnorm, v) - g_bias) * (1.0 / (1.0 + d)) * g_intensity;
}

void main() {
  vec2 texCoord = gl_FragCoord.xy / Screen;

  vec2 vec[4];
  vec[0] = vec2(1,0);
  vec[1] = vec2(-1,0);
  vec[2] = vec2(0,1);
  vec[3] = vec2(0,-1);

  vec3 p = texture(gPosition, texCoord).xyz;
  vec3 n = texture(gNormal, texCoord).xyz;
  vec2 r = texture(Noise, texCoord / (Screen / textureSize(Noise, 0))).xy;

  float ao = 0.0f;
  float rad = g_sample_rad / p.z;

  //**SSAO Calculation**//
  int iterations = 4;
  for (int j = 0; j < iterations; ++j)
  {
    vec2 coord1 = reflect(vec[j], r) * rad;
    vec2 coord2 = vec2(coord1.x * 0.707 - coord1.y * 0.707,
  			  coord1.x * 0.707 + coord1.y * 0.707);

    ao += doAmbientOcclusion(texCoord, coord1 * 0.25, p, n);
    ao += doAmbientOcclusion(texCoord, coord2 * 0.5, p, n);
    ao += doAmbientOcclusion(texCoord, coord1 * 0.75, p, n);
    ao += doAmbientOcclusion(texCoord, coord2, p, n);
  }
  ao /= float(iterations) * 4.0;
  //**END**//

  //Do stuff here with your occlusion value Ã¢aoÃ¢: modulate ambient lighting, write it to a buffer for later //use, etc.
  ssaoFactor = ao;
}
