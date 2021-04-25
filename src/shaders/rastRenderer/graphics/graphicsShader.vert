#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../../uniformBufferObject.glsl"
#include "../../material.glsl"
#include "../../light.glsl"
#include "../../objectInfo.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject camera; };
layout(binding = 1) readonly buffer MaterialArray { Material[] materials; };
layout(push_constant) uniform ObjectInfoStruct { ObjectInfo objectInfo; };

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in int inMaterialIndex;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;
layout(location = 3) out flat int outMaterialIndex;
layout(location = 4) out vec3 outViewPos;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
    gl_Position = camera.projMat * camera.viewMat * objectInfo.transform * vec4(inPosition,1);

	outPos = vec3(objectInfo.transform * vec4(inPosition, 1.0));
	outNormal = vec3(transpose(inverse(objectInfo.transform)) * vec4(inNormal, 0.0));
	outTexCoord = inTexCoord;
	outMaterialIndex = objectInfo.materialOffset+inMaterialIndex;

	outViewPos = camera.viewMatInverse[3].xyz;
}
