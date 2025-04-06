perFrame mat4 uProjection;
perFrame mat4 uView;
//perFrame samplerCube uEnvironmentMap;

perVertex vec3 vTexCoords;

vec4 vertex(vec3 iVertex, vec3 iNormal, vec2 iUV) {
    // Add small dependency between vertex and normal/uv to avoid compilation error
    vTexCoords = (iVertex + 1.0) / 2.0;
    vTexCoords += iNormal / 10000.0f + vec3(iUV,0) / 10000.0f;

    mat4 rotView = mat4(mat3(uView)); // Remove translation from the view matrix
    return uProjection * rotView * vec4(iVertex, 1.0);
}

void fragment(out vec4 color) {
    //return vec4((vTexCoords + 1.0)/2.0, 1.0);
    color = vec4(vTexCoords, 1.0);
}
