#version 300 es
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoords;

out vec2 TexCoords;
out vec3 Position;

void main()
{
    TexCoords = inTexCoords;
    Position = inPosition;
    gl_Position = vec4(inPosition, 1.0f);
}
