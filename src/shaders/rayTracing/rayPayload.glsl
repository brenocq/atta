//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// rayPayload.glsl
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef RAY_PAYLOAD_GLSL
#define RAY_PAYLOAD_GLSL
#include "lights/base.glsl"

struct RayPayload
{
	float t;
	Interaction it;
};

struct RayPayload1
{
	float t;
	vec3 gn;
	vec3 sn;
	vec3 point;
	vec3 direction;

	uint materialType;
	float materialData[20];
};

#endif// RAY_PAYLOAD_GLSL
