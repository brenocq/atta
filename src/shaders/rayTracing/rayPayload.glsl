//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// rayPayload.rchit
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
struct RayPayload
{
	vec4 colorAndDistance;// rgb + t
	vec4 scatterDirection;// xyz + w (is scatter needed)
	uint randomSeed;
};
