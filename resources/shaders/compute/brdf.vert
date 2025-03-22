#version 300 es
//in vec3 aPos;
//in vec2 aTexCoords;
in vec3 inPos;
in vec3 inNormal;
in vec2 inTexCoords;

out vec2 TexCoords;
out vec3 Pos;

void main()
{
    TexCoords = inTexCoords;
    Pos = inPos;
    gl_Position = vec4(inPos, 1.0);
}
