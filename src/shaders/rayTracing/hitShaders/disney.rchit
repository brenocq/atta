//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disney.rchit
// Date: 2021-07-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#version 460
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : require

#include "../../objectInfo.glsl"
#include "../../material.glsl"
#include "../../light.glsl"
#include "../uniformBufferObject.glsl"
#include "../rayPayload.glsl"

layout(binding = 0, set = 0) uniform accelerationStructureEXT scene;
layout(binding = 3) readonly uniform UniformBufferObjectStruct { UniformBufferObject ubo; };
layout(binding = 4) readonly buffer VertexBuffer { float Vertices[]; };
layout(binding = 5) readonly buffer IndexBuffer { uint Indices[]; };
layout(binding = 6) readonly buffer MaterialBuffer { Material[] materialBuffer; };
layout(binding = 7) readonly buffer ObjectInfoBuffer { ObjectInfo[] Objects; };
layout(binding = 8) readonly buffer LightStruct { Light[] lightBuffer; };
layout(binding = 9) uniform sampler2D[] textures; 
hitAttributeEXT vec3 hitAttributes;
rayPayloadInEXT RayPayload ray;
layout(location = 1) rayPayloadEXT bool isShadowed;

#include "../vertex.glsl"
#include "base.glsl"
#include "../bsdf/disney.glsl"
#include "../lights/light.glsl"

void main()
{
	//---------- Precomputations ----------//
	Interaction it = populateInteraction();
	BSDF bsdf;
	bsdf.eta = ray.eta;
	bsdf.ng = it.ng;
	bsdf.ns = it.ns;
	bsdf.ss = it.ss;
	bsdf.ts = it.ts;
	bsdf.bxdf = MaterialToBXDF(materialBuffer[it.materialIndex], it.texCoord);

	//---------- Calculate contribution ----------//
	ray.t = gl_HitTEXT;
	ray.origin = it.worldPos;

	// Update throughput based on medium absoption
	ray.pathThroughput *= exp(-vec3(0,0,0)*ray.t);

	uint type = BXDF_FLAG_ALL;

	// Uniform sample lights 
	float uLightIndex = randomFloat(ray.seed);
	vec2 uLight = vec2(randomFloat(ray.seed), randomFloat(ray.seed));
	vec2 uScattering = vec2(randomFloat(ray.seed), randomFloat(ray.seed));
	ray.radiance = ray.pathThroughput*Light_uniformSampleOneLight(bsdf, it, ubo.nLights, uLightIndex, uLight, uScattering);

	// Sample next direction
	vec3 bsdfF;
	float bsdfPdf;
	bsdfF = BSDF_sampleF(bsdf, it.wo, ray.wi, uScattering, bsdfPdf, type);

	// Update throughput based on reflectance
	ray.pathThroughput *= bsdfF * abs(dot(it.ns, ray.wi))/bsdfPdf;
}
