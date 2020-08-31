#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../rayTracing/material.glsl"
#include "../rayTracing/uniformBufferObject.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject Camera; };
layout(binding = 1) readonly buffer MaterialArray { Material[] Materials; };
layout(push_constant) uniform ObjectInfo {
  mat4 modelView;
  vec3 color;
} objectInfo;

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InTexCoord;
layout(location = 3) in int InMaterialIndex;

layout(location = 0) out vec3 FragColor;
layout(location = 1) out vec3 FragNormal;
layout(location = 2) out vec2 FragTexCoord;
layout(location = 3) out flat int FragMaterialIndex;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	Material m = Materials[InMaterialIndex];

    gl_Position = Camera.projection * Camera.modelView * objectInfo.modelView * vec4(InPosition, 1.0);
	if(objectInfo.color.x + objectInfo.color.y + objectInfo.color.z == 0)
    	FragColor = m.diffuse.xyz;
	else
    	FragColor = objectInfo.color;
	FragNormal = vec3(Camera.modelView * vec4(InNormal, 0.0)); // technically not correct, should be ModelInverseTranspose
	FragTexCoord = InTexCoord;
	FragMaterialIndex = InMaterialIndex;
}
