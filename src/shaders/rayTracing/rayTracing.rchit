#version 460
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#extension GL_NV_ray_tracing : require
#include "material.glsl"

layout(binding = 4) readonly buffer VertexArray { float Vertices[]; };
layout(binding = 5) readonly buffer IndexArray { uint Indices[]; };
layout(binding = 6) readonly buffer MaterialArray { Material[] Materials; };
layout(binding = 7) readonly buffer OffsetArray { uvec2[] Offsets; };
layout(binding = 8) uniform sampler2D[] TextureSamplers;

#include "scatter.glsl"
#include "vertex.glsl"

hitAttributeNV vec2 hitAttributes;
rayPayloadInNV RayPayload ray;

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
	const uvec2 offsets = Offsets[gl_InstanceCustomIndexNV];
	const uint indexOffset = offsets.x;
	const uint vertexOffset = offsets.y;
	const Vertex v0 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 0]);
	const Vertex v1 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 1]);
	const Vertex v2 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 2]);
	const Material material = Materials[v0.materialIndex];

	// Compute the ray hit point properties.
	const vec3 barycentrics = vec3(1.0 - hitAttributes.x - hitAttributes.y, hitAttributes.x, hitAttributes.y);
	const vec3 normal = normalize(mixNormals(v0.normal, v1.normal, v2.normal, barycentrics));
	const vec2 texCoord = mixNormals(v0.texCoord, v1.texCoord, v2.texCoord, barycentrics);

	ray = scatter(material, gl_WorldRayDirectionNV, normal, texCoord, gl_HitTNV, ray.randomSeed);
}
