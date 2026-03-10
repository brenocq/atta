perFrame mat4 uView;
perFrame mat4 uProjection;
perFrame vec3 uCamPos;

perVertex vec4 vColor;
perVertex vec3 vFragPos;

vec4 vertex(vec3 iPos, vec4 iColor, float iWidth) {
    vec4 pos = uProjection * uView * vec4(iPos, 1.0);
    vColor = iColor;
    vFragPos = iPos;
    return vec4(pos.xyz, pos.w);
}

void fragment(out vec4 color) {
    float dist = length(uCamPos - vFragPos) + 0.0001;
    color = vColor;

    // Parameters for controlling the fade effect
    float fadeStart = 20.0; // Distance where the fading starts
    float fadeEnd = 50.0;   // Distance where the grid is fully faded/transparent

    // Clamped ratio between where fading starts and where it ends
    float fadeRatio = clamp((dist - fadeStart) / (fadeEnd - fadeStart), 0.0, 1.0);

    // Apply fade falloff
    color.a *= 1.0 - fadeRatio;
}
