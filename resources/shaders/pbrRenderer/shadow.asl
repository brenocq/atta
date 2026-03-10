perFrame mat4 lightSpaceMatrix;
perDraw mat4 model;

vec4 vertex(vec3 iPos, vec3 iNormal, vec2 iUV) {
    vec4 pos = lightSpaceMatrix * model * vec4(iPos, 1.0);
#ifdef ATTA_OPENGL
    // atta's orthographic outputs Z in [0,1] (Vulkan-style).
    // OpenGL expects Z_ndc in [-1,1] and remaps to depth as d=(z+1)/2.
    // Remap so the depth buffer stores the same [0,1] range that
    // pbrRenderer.asl reads back via posDirectionalLightSpace.z.
    pos.z = pos.z * 2.0 - 1.0;
#endif
    return pos;
}

// Depth-only pass: no color output needed.
// gl_FragCoord.z is automatically written to the depth buffer by the rasterizer.
void fragment() {}
