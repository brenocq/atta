#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../uniformBufferObject.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject ubo; };
layout(location = 0) out vec3 outPos;

vec3 positions[36] = vec3[](
	vec3(1.0, 1.0, -1.0),
	vec3(-1.0, 1.0, -1.0),
	vec3(-1.0, 1.0, 1.0),
	vec3(1.0, 1.0, -1.0),
	vec3(-1.0, 1.0, 1.0),
	vec3(1.0, 1.0, 1.0),

	vec3(1.0, -1.0, 1.0),
	vec3(1.0, 1.0, 1.0),
	vec3(-1.0, 1.0, 1.0),
	vec3(1.0, -1.0, 1.0),
	vec3(-1.0, 1.0, 1.0),
	vec3(-1.0, -1.0, 1.0),

	vec3(-1.0, -1.0, 1.0),
	vec3(-1.0, 1.0, 1.0),
	vec3(-1.0, 1.0, -1.0),
	vec3(-1.0, -1.0, 1.0),
	vec3(-1.0, 1.0, -1.0),
	vec3(-1.0, -1.0, -1.0),

	vec3(-1.0, -1.0, -1.0),
	vec3(1.0, -1.0, -1.0),
	vec3(1.0, -1.0, 1.0),
	vec3(-1.0, -1.0, -1.0),
	vec3(1.0, -1.0, 1.0),
	vec3(-1.0, -1.0, 1.0),

	vec3(1.0, -1.0, -1.0),
	vec3(1.0, 1.0, -1.0),
	vec3(1.0, 1.0, 1.0),
	vec3(1.0, -1.0, -1.0),
	vec3(1.0, 1.0, 1.0),
	vec3(1.0, -1.0, 1.0),

	vec3(-1.0, -1.0, -1.0),
	vec3(-1.0, 1.0, -1.0),
	vec3(1.0, 1.0, -1.0),
	vec3(-1.0, -1.0, -1.0),
	vec3(1.0, 1.0, -1.0),
	vec3(1.0, -1.0, -1.0)
);

void main() 
{
	vec3 inPos = positions[gl_VertexIndex];
	outPos = inPos;
	outPos.xy *= -1.0;

	mat4 view = ubo.viewMat;
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;

    gl_Position = ubo.projMat * view * vec4(inPos,1);
}
