perPass mat4 model;
perPass vec3 lightPos;
perPass float far_plane;
perPass mat4 shadowMatrices[6];

perVertex vec4 FragPos;

void vertex(in vec3 inPosition, in vec3 inNormal, in vec2 inTexCoord, out vec4 position) { position = model * vec4(inPosition, 1.0); }

void geometry(in(triangles), out(triangle_strip, max_vertices = 18)) {
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;            // built-in variable that specifies to which face we render.
        for (int i = 0; i < 3; i++) // for each triangle vertex
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}

void fragment(out vec4 color, out float depth) {
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    depth = lightDistance;
}
