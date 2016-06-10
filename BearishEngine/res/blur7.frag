#version 330

out vec4 color;

uniform sampler2D diffuse;

in vec2 texCoords0[7];

void main() {
    color = vec4(0.0);
    color += texture2D(diffuse, texCoords0[0]) * 0.0776744219933;
    color += texture2D(diffuse, texCoords0[1]) * 0.115876621105;
    color += texture2D(diffuse, texCoords0[2]) * 0.147308056121;
    color += texture2D(diffuse, texCoords0[3]) * 0.159576912161;
    color += texture2D(diffuse, texCoords0[4]) * 0.147308056121;
    color += texture2D(diffuse, texCoords0[5]) * 0.115876621105;
    color += texture2D(diffuse, texCoords0[6]) * 0.0776744219933;
}
