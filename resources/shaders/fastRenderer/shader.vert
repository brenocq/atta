#version 450 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec3 color;

void main()
{
    gl_Position = projection * view * transform * vec4(inPosition, 1.0f);
    color = vec3(inNormal);
}
