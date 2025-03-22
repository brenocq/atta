#version 300 es
in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat3 environmentMapOri;

out vec3 WorldPos;

void main()
{
    WorldPos = aPos;

    mat4 rotView = mat4(mat3(view)*environmentMapOri);
    vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

    gl_Position = clipPos.xyww;
}
