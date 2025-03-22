perFrame mat4 uView;
perFrame mat4 uProjection;
perDraw mat4 uModel;
perDraw vec3 uAlbedo;
perDraw sampler2D uAlbedoTexture;

perVertex vec2 vUV;

vec4 vertex(vec3 iVertex, vec3 iNormal, vec2 iUV) {
    vUV = iUV;
    float dummyNormal = iNormal.x / 10000.0f + 1.0f;// To avoid being optimized out
    return uProjection * uView * uModel * vec4(iVertex, 1.0f) * dummyNormal;
}

void fragment(out vec4 color) {
    if (uAlbedo.x < 0.0f) {
        color = texture(uAlbedoTexture, vUV).rgba;
        if (color.a < 0.1)
            discard;
        return;
    }

    color = vec4(uAlbedo, 1.0f);
}
