#version 460
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

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out flat int fragMaterialIndex;
layout(location = 4) out vec3 fragPos;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	Material m = materials[inMaterialIndex];

	fragPos = vec3(objectInfo.modelView * vec4(inPosition, 1.0));
	fragNormal = vec3(transpose(inverse(objectInfo.modelView)) * vec4(inNormal, 1.0));

    gl_Position = Camera.projection * Camera.modelView * objectInfo.modelView * vec4(inPosition, 1.0);
    fragColor = m.diffuse.xyz * objectInfo.color;

	if(objectInfo.materialIndex >0 -1)
		fragMaterialIndex = objectInfo.materialIndex;
	else
		fragMaterialIndex = inMaterialIndex;

	fragTexCoord = inTexCoord;
}
