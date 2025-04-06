perFrame mat4 uProjection;
perFrame mat4 uView;
perFrame samplerCube uEnvironmentMap;

perVertex vec3 vTexCoords;

vec4 vertex(vec3 iVertex, vec3 iNormal, vec2 iUV) {
    vTexCoords = iVertex;
    // Add small dependency between vertex and normal/uv to avoid compilation error
    vTexCoords += iNormal / 10000.0f + vec3(iUV,0) / 10000.0f;

    // Remove translation from the view matrix (so the skybox remains centered)
    mat4 rotView = mat4(mat3(uView));
    return uProjection * rotView * vec4(iVertex, 1.0);
}

void fragment(out vec4 color) {
    // Sample the cubemap using the normalized direction
    color = texture(uEnvironmentMap, normalize(vTexCoords));
}
