//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// rayTracing.rchit
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#version 460
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_ray_tracing : require

#include "../material.glsl"
#include "../objectInfo.glsl"
#include "rayPayload.glsl"
#include "materials/material.glsl"
#include "lights/base.glsl"

layout(binding = 4) readonly buffer VertexArray { float Vertices[]; };
layout(binding = 5) readonly buffer IndexArray { uint Indices[]; };
layout(binding = 6) readonly buffer MaterialArray { MaterialData[] Materials; };
//layout(binding = 7) uniform sampler2D[] textures;
layout(binding = 7) readonly buffer ObjectInfoArray { ObjectInfo[] Objects; };

#include "vertex.glsl"

hitAttributeEXT vec3 hitAttributes;
rayPayloadInEXT RayPayload ray;

vec2 mixCoords(vec2 a, vec2 b, vec2 c, vec3 barycentrics)
{
	return a * barycentrics.x + b * barycentrics.y + c * barycentrics.z;
}

vec3 mixNormals(vec3 a, vec3 b, vec3 c, vec3 barycentrics) 
{
    return a * barycentrics.x + b * barycentrics.y + c * barycentrics.z;
}

void main()
{
	//const ObjectInfo object = Objects[gl_InstanceCustomIndexEXT];
	const ObjectInfo object = Objects[gl_InstanceID];
	const uint indexOffset = object.indexOffset;
	const uint vertexOffset = object.vertexOffset;
	const Vertex v0 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 0]);
	const Vertex v1 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 1]);
	const Vertex v2 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 2]);
	MaterialData material = Materials[object.materialOffset];// Change to materialOffset from Instance buffer

	// Compute the ray hit point properties
	const vec3 barycentrics = vec3(1.0 - hitAttributes.x - hitAttributes.y, hitAttributes.x, hitAttributes.y);

	// Compute normal
	vec3 normal = mixNormals(v0.normal, v1.normal, v2.normal, barycentrics);
	normal = normalize(vec3(inverse(transpose(object.transform))*vec4(normal, 0.0)));

	// Compute world pos
 	vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;	
	//worldPos = vec3(Instances[gl_InstanceID].transform* vec4(worldPos, 1.0));

	// Compute texCoord
	const vec2 texCoord = mixCoords(v0.texCoord, v1.texCoord, v2.texCoord, barycentrics);

	ray.t = gl_HitTEXT;

	// Populate BSDF from intersection point
	BSDF bsdf;
	bsdf.eta = 1;
	bsdf.ng = normal;
	bsdf.ns = normal;
	bsdf.ss = v0.position - v1.position;
	bsdf.ts = v1.position - v2.position;
	//bsdf.bxdf.type = BXDF_TYPE_LAMBERTIAN_REFLECTION;
	bsdf.bxdf.type = BXDF_TYPE_OREN_NAYAR;

	Interaction it;
	it.point = worldPos;
	it.wo = -gl_WorldRayDirectionEXT;
	it.n = normal;
	it.bsdf = bsdf;

	ray.it = it;
}
