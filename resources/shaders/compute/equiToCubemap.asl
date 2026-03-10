perFrame mat4 uProjection;
perFrame mat4 uView;
perFrame sampler2D uEquirectangularMap;

perVertex vec3 localPos;

vec4 vertex(vec3 iPos, vec3 iNormal, vec2 iUV) {
    localPos = iPos;
    // Add small dependency between vertex and normal/uv to avoid compilation error
    localPos += iNormal / 10000.0f + vec3(iUV,0) / 10000.0f;
    return uProjection * uView * vec4(localPos, 1.0);
}

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v) {
    // Z-up convention: azimuth in XY plane, elevation along Z axis
    vec2 uv = vec2(atan(v.y, v.x), asin(v.z));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void fragment(out vec4 color) {
    vec2 uv = sampleSphericalMap(normalize(localPos));
    color = vec4(texture(uEquirectangularMap, uv).rgb, 1.0);
}
