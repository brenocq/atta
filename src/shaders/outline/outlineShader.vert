#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../rayTracing/material.glsl"
#include "../rayTracing/uniformBufferObject.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject Camera; };
layout(binding = 1) readonly buffer MaterialArray {
	Material[] materials; 
};
layout(push_constant) uniform ObjectInfo {
  mat4 modelView;
  vec3 color;
  int materialIndex;
} objectInfo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in int inMaterialIndex;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	vec3 fragPos = vec3(objectInfo.modelView * vec4(inPosition, 1.0));
	vec3 fragNormal = normalize(vec3(transpose(inverse(objectInfo.modelView)) * vec4(inNormal, 1.0)));

	// Extrude along normal
	vec4 pos = vec4(fragPos.xyz + fragNormal * 0.01, 1.0);
	gl_Position = Camera.projection * Camera.modelView * pos;
}
