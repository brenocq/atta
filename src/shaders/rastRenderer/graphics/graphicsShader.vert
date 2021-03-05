#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../../uniformBufferObject.glsl"
#include "../../material.glsl"
#include "../../objectInfo.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct
{ 
	UniformBufferObject camera; 
};

layout(binding = 1) readonly buffer MaterialArray
{
	Material[] materials; 
};

layout(push_constant) uniform ObjectInfoStruct
{
	ObjectInfo objectInfo;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in int inMaterialIndex;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;
layout(location = 3) out flat int outMaterialIndex;
layout(location = 4) out vec3 outViewPos;

vec3 ExtractCameraPos(mat4 a_modelView)
{
  mat4 modelViewT = transpose(a_modelView);

  // Get plane normals
  vec3 n1 = modelViewT[0].xyz;
  vec3 n2 = modelViewT[1].xyz;
  vec3 n3 = modelViewT[2].xyz;

  // Get plane distances
  float d1 = modelViewT[0].w;
  float d2 = modelViewT[1].w;
  float d3 = modelViewT[2].w;

  // Get the intersection of these 3 planes
  vec3 n2n3 = cross(n2, n3);
  vec3 n3n1 = cross(n3, n1);
  vec3 n1n2 = cross(n1, n2);

  vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
  float denom = dot(n1, n2n3);

  return top / -denom;
}

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
    gl_Position = camera.projMat * camera.viewMat * objectInfo.transform * vec4(inPosition,1);

	outPos = vec3(objectInfo.transform * vec4(inPosition, 1.0));
	outNormal = vec3(transpose(inverse(objectInfo.transform)) * vec4(inNormal, 1.0));
	outTexCoord = inTexCoord;
	outMaterialIndex = objectInfo.materialOffset;

	outViewPos = ExtractCameraPos(camera.viewMat);
}
