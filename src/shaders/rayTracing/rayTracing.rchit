#version 460
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_ray_tracing : require
#include "material.glsl"
#include "instanceInfo.glsl"

layout(binding = 4) readonly buffer VertexArray { float Vertices[]; };
layout(binding = 5) readonly buffer IndexArray { uint Indices[]; };
layout(binding = 6) readonly buffer MaterialArray { Material[] Materials; };
layout(binding = 7) readonly buffer OffsetArray { uvec2[] Offsets; };
layout(binding = 8) readonly buffer InstanceArray { InstanceInfo[] Instances; };
layout(binding = 9) uniform sampler2D[] TextureSamplers;

#include "scatter.glsl"
#include "vertex.glsl"

hitAttributeEXT vec3 hitAttributes;
rayPayloadInEXT RayPayload ray;

vec2 mixNormals(vec2 a, vec2 b, vec2 c, vec3 barycentrics)
{
	return a * barycentrics.x + b * barycentrics.y + c * barycentrics.z;
}

vec3 mixNormals(vec3 a, vec3 b, vec3 c, vec3 barycentrics) 
{
    return a * barycentrics.x + b * barycentrics.y + c * barycentrics.z;
}

void main()
{
	// Get the material.
	//const uvec2 offsets = Offsets[gl_InstanceCustomIndexNV];
	//const uint indexOffset = offsets.x;
	//const uint vertexOffset = offsets.y;
	//const Vertex v0 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 0]);
	//const Vertex v1 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 1]);
	//const Vertex v2 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 2]);
	//Material material = Materials[v0.materialIndex];
	//if(Instances[gl_InstanceID].diffuse.x != -1)
	//{
	//	material.diffuse = Instances[gl_InstanceID].diffuse;
	//}

	//// Compute the ray hit point properties.
	//const vec3 barycentrics = vec3(1.0 - hitAttributes.x - hitAttributes.y, hitAttributes.x, hitAttributes.y);

	//// Compute normal
	//vec3 normal = mixNormals(v0.normal, v1.normal, v2.normal, barycentrics);
	//normal = normalize(vec3(Instances[gl_InstanceID].transformIT*vec4(normal, 0.0)));

	//// Compute world pos
 	vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;	
	//worldPos = vec3(Instances[gl_InstanceID].transform* vec4(worldPos, 1.0));

	//const vec2 texCoord = mixNormals(v0.texCoord, v1.texCoord, v2.texCoord, barycentrics);

	//ray = scatter(material, gl_WorldRayDirectionNV, normal, texCoord, gl_HitTNV, ray.randomSeed);
	////int transform = int(Instances[gl_InstanceID].transform[0][0]);
	////vec4 diffuse = Instances[gl_InstanceID].diffuse;
	////ray.colorAndDistance = vec4((transform>>2)%2,(transform>>1)%2,transform%2,0);
	////ray.colorAndDistance = diffuse;
	ray.colorAndDistance = vec4(worldPos,gl_HitTEXT);
}
