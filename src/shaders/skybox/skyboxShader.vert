#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../material.glsl"
#include "../objectInfo.glsl"
#include "../uniformBufferObject.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct
{ 
	UniformBufferObject camera;
};

//layout(binding = 1) readonly buffer MaterialArray { Material[] Materials; };
layout(push_constant) uniform ObjectInfoStruct {
  //ObjectInfo objectInfo;
  mat4 transform;
} objectInfo;

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InTexCoord;
layout(location = 3) in int InMaterialIndex;

layout(location = 0) out vec3 FragColor;
layout(location = 1) out vec3 FragPos;

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

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	const vec3 viewPos = ExtractCameraPos(camera.viewMat);
	const vec4 fragPos = camera.projMat * camera.viewMat * objectInfo.transform * vec4(InPosition*1000.0f, 1.0);
    gl_Position = fragPos;

    FragColor = InNormal;
  	FragPos = (objectInfo.transform * vec4(InPosition*10.0f, 1.0)).xyz;
}
