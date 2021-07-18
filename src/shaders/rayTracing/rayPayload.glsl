//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// rayPayload.glsl
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef RAY_PAYLOAD_GLSL
#define RAY_PAYLOAD_GLSL

struct RayPayload
{
	// Ray intersection
	vec3 origin;
	float t;// TODO maybe not needed
	vec3 radiance;// BSDF+light radiance evaluation
	// BSDF
	vec3 wi;// BSDF sampled direction
	// Ray state
	float eta;// Index of refraction
	vec3 pathThroughput;// Path tracer throughput
	uint seed;// XXX For now it is here, but should be a memory vector
};

#endif// RAY_PAYLOAD_GLSL
