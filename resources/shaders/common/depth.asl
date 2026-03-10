perFrame mat4 uView;
perFrame mat4 uProjection;
perDraw mat4 uModel;
perDraw vec4 uColor;
// uniform sampler2D uDepthMap;

perVertex vec3 vUV;

vec4 vertex(vec3 iVertex, vec3 iNormal, vec3 iUV) {
    vUV = iUV;
    return uProjection * uView * uModel * vec4(iVertex, 1.0f);
}

vec4 fragment() {
    // float depthValue = texture(depthMap, vUV).r;
    // return vec4(vec3(depthValue), 1.0f);
    return vec4(0, 0, 0, 1);
}
