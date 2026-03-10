perFrame mat4 lightSpaceMatrix;
perDraw mat4 model;

vec4 vertex(vec3 iPos, vec3 iNormal, vec2 iUV) {
    return lightSpaceMatrix * model * vec4(iPos, 1.0);
}

// Depth-only pass: no color output needed.
// gl_FragCoord.z is automatically written to the depth buffer by the rasterizer.
void fragment() {}
