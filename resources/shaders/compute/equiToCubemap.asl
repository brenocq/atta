perFrame mat4 uProjection;
perFrame mat4 uView;
perFrame sampler2D uEquirectangularMap;

perVertex vec3 localPos;

vec4 vertex(vec3 iPos, vec3 iNormal, vec2 iUV) {
    localPos = iPos;
    return uProjection * uView * vec4(localPos, 1.0);
}

vec4 fragment() {
    vec2 uv = sampleSphericalMap(normalize(localPos));
    return vec4(texture(uEquirectangularMap, uv).rgb, 1.0);
}

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
