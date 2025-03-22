perFrame mat4 uView;
perFrame mat4 uProjection;
perFrame samplerCube uEnvironmentMap;

perVertex vLocalPos;

vec4 vertex(vec3 iVertex, vec3 iNormal, vec3 iUV) {
    vLocalPos = iVertex;
    mat4 rotView = mat4(mat3(view)); // Remove translation from the view matrix
    vec4 clipPos = projection * rotView * vec4(iVertex, 1.0);
    return clipPos.xyww;
}

vec4 fragment() {
    vec3 envColor = texture(uEnvironmentMap, vLocalPos).rgb;

    // Gamma correction
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    return vec4(envColor, 1.0);
}
