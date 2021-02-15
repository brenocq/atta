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

#endif// RAY_PAYLOAD_GLSL
