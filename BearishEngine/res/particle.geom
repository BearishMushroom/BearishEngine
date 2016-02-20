#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 MVP;
uniform vec3 eyePos;
uniform vec3 up;

in vec3 position0[];
in vec3 scale0[];
in float rotation0[];
in vec4 color0[];

out vec2 texCoord1;
out vec4 color1;

void main() {
  mat4 rotationMatrix = mat4(cos(rotation0[0]), -sin(rotation0[0]), 0.0, 0.0,
                             sin(rotation0[0]), cos(rotation0[0]), 0.0, 0.0,
                             0.0, 0.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 1.0);

  vec3 toCamera = normalize(eyePos - position0[0]);
  vec3 right = cross(toCamera, up);

  vec3 startPos = position0[0];

  //startPos -= right * (scale0[0].x / 2);
  startPos -= right * 0.5 * scale0[0];
  gl_Position = MVP * vec4(startPos, 1.0);
  texCoord1 = vec2(0, 0);
  color1 = color0[0];
  EmitVertex();

  //tartPos.y += scale0[0].x;
  startPos += up * scale0[0];
  gl_Position = MVP * vec4(startPos, 1.0);
  texCoord1 = vec2(0, 1);
  color1 = color0[0];
  EmitVertex();

  //startPos.y -= scale0[0].x;
  startPos -= up * scale0[0];
  //startPos += right * scale0[0];
  startPos += right * scale0[0];
  gl_Position = MVP * vec4(startPos, 1.0);
  texCoord1 = vec2(1, 0);
  color1 = color0[0];
  EmitVertex();

  //startPos.y += scale0[0].x;
  startPos += up * scale0[0];
  gl_Position = MVP * vec4(startPos, 1.0);
  texCoord1 = vec2(1, 1);
  color1 = color0[0];
  EmitVertex();

  EndPrimitive();
}
