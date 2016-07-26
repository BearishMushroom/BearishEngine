#version 330

out vec2 depth;

void main() {
/*	float d = gl_FragCoord.z;
	float dx = dFdx(d);
	float dy = dFdy(d);

	float moment = d * d + 0.25 * (dx * dx + dy * dy);

	depth = vec2(d, moment);*/

	depth = vec2(gl_FragCoord.z);
}
