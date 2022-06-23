#version 300 es
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    texCoord = inTexCoord;
}
