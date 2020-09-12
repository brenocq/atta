#version 460
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#extension GL_NV_ray_tracing : require
#include "material.glsl"

layout(binding = 4) readonly buffer VertexArray { float Vertices[]; };
layout(binding = 5) readonly buffer IndexArray { uint Indices[]; };
layout(binding = 6) readonly buffer MaterialArray { Material[] Materials; };
layout(binding = 7) readonly buffer OffsetArray { uvec2[] Offsets; };
layout(binding = 9) uniform sampler2D[] TextureSamplers;
layout(binding = 10) readonly buffer SphereArray { vec4[] Spheres; };

#include "scatter.glsl"
#include "vertex.glsl"

hitAttributeNV vec4 sphere;
rayPayloadInNV RayPayload ray;

vec2 getSphereTexCoord(const vec3 point)
{
	const float phi = atan(point.x, point.z);
	const float theta = asin(point.y);
	const float pi = 3.14159265358979323846264338327950288419716939937510;

	return vec2
	(
		(phi + pi) / (2* pi),
		1 - (theta + pi /2) / pi
	);
}

void main()
{
	// Get the material.
	const uvec2 offsets = Offsets[gl_InstanceCustomIndexNV];
	const uint indexOffset = offsets.x;
	const uint vertexOffset = offsets.y;
	const Vertex v0 = unpackVertex(vertexOffset + Indices[indexOffset]);
	const Material material = Materials[v0.materialIndex];

	// Compute the ray hit point properties.
	const vec4 sphere = Spheres[gl_InstanceCustomIndexNV];
	const vec3 center = sphere.xyz;
	const float radius = sphere.w;
	const vec3 point = gl_WorldRayOriginNV + gl_HitTNV * gl_WorldRayDirectionNV;
	const vec3 normal = (point - center) / radius;
	const vec2 texCoord = getSphereTexCoord(normal);

	ray = scatter(material, gl_WorldRayDirectionNV, normal, texCoord, gl_HitTNV, ray.randomSeed);
}
