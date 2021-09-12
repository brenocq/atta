#version 450 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
out vec3 color;

void main()
{
	gl_Position = vec4(inPosition.xy, 0.0f, 1.0f);
	color = vec3(inNormal);
}
