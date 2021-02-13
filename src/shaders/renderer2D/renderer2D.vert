#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "uniformBufferObject.glsl"
#include "../objectInfo.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct 
{ 
	UniformBufferObject camera;
};

layout(push_constant) uniform ObjectInfoStruct {
  ObjectInfo objectInfo;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in int inMaterialIndex;

layout(location = 0) out flat int outMaterialIndex;
layout(location = 1) out vec2 outTexCoord;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	vec3 fragPos = vec3(camera.orthoMatrix * camera.viewMatrix * objectInfo.modelMat * vec4(inPosition, 1.0));

	fragPos.y = -fragPos.y;// Invert Y because vulkan Y coord points down
	gl_Position = vec4(fragPos.xyz, 1);

	outTexCoord = inTexCoord;
	outMaterialIndex = objectInfo.materialOffset;
}
