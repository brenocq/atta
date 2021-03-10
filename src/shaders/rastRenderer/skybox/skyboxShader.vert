#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../../uniformBufferObject.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject ubo; };

layout(location = 0) out vec2 outFragPos;

vec2 positions[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(1.0, 1.0)
);


void main() 
{
	const vec2 fragPos = positions[gl_VertexIndex];
    gl_Position = vec4(fragPos,0.9999999,1);

	outFragPos = fragPos;
}
