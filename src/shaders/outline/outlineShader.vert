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
  // http://paulbourke.net/geometry/3planes/
  vec3 n2n3 = cross(n2, n3);
  vec3 n3n1 = cross(n3, n1);
  vec3 n1n2 = cross(n1, n2);

  vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
  float denom = dot(n1, n2n3);

  return top / -denom;
}

void main() 
{
	vec3 viewPos = ExtractCameraPos(Camera.modelView);

	vec3 fragPos = vec3(objectInfo.modelView * vec4(inPosition, 1.0));
	vec3 fragNormal = normalize(vec3(transpose(inverse(objectInfo.modelView)) * vec4(inNormal, 1.0)));

	// Extrude along normal
	float off = length(viewPos-fragPos);
	vec4 pos = vec4(fragPos.xyz + fragNormal * 0.005 * off, 1.0);
	gl_Position = Camera.projection * Camera.modelView * pos;
}
