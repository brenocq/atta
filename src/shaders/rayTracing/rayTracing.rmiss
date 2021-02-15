//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// rayTracing.rmiss
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#version 460
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_ray_tracing : require
#include "rayPayload.glsl"
#include "uniformBufferObject.glsl"

layout(binding = 3) readonly uniform UniformBufferObjectStruct { UniformBufferObject camera; };

layout(location = 0) rayPayloadInEXT RayPayload ray;

void main()
{
	//if (camera.hasSky)
	//{
	//	// Sky color
	//	const float t = 0.5*(normalize(gl_WorldRayDirectionNV).y + 1);
	//	const vec3 skyColor = mix(vec3(1.0), vec3(0.5, 0.7, 1.0), t);

	//	ray.colorAndDistance = vec4(skyColor, -1);
	//}
	//else
	//{
	//	ray.colorAndDistance = vec4(0.8, 0.8, 0.8, -1);
	//}

	ray.t = -1;
}
