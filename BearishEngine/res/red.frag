#version 330 core

out vec4 fragcolor;

uniform sampler2D gTexCoord;

uniform vec2 screen;
uniform int matID;

in mat4 lightMat;

vec2 CalcTexCoord(vec2 screenSize) {
  return gl_FragCoord.xy / screenSize;
}

void main() {
	vec2 tc = CalcTexCoord(screen);
	int mat = int(texture(gTexCoord, tc).z);

	if (mat == matID) {
    fragcolor = vec4(1, 0, 0, 1);
	} else {
		discard;
	}
}
