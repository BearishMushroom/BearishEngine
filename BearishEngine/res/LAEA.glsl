vec2 EncodeNormal(vec3 normal) {
    float f = sqrt(8*normal.z+8);
    return normal.xy / f + 0.5;
}

vec3 DecodeNormal(vec2 encodedNormal) {
    vec2 fenc = encodedNormal*4-2;
    float f = dot(fenc,fenc);
    float g = sqrt(1-f/4);
    vec3 n;
    n.xy = fenc*g;
    n.z = 1-f/2;
    return n;
}
