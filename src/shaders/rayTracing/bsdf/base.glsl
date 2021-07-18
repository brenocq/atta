//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// base.glsl
// Date: 2021-07-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BSDF_BASE_GLSL
#define BSDF_BASE_GLSL

vec3 BSDF_worldToLocal(BSDF bsdf, vec3 v)
{
	return vec3(dot(v, bsdf.ss), dot(v, bsdf.ts), dot(v, bsdf.ns));
}

vec3 BSDF_localToWorld(BSDF bsdf, vec3 v)
{
	vec3 ss = bsdf.ss;
	vec3 ts = bsdf.ts;
	vec3 ns = bsdf.ns;
	return vec3(ss.x * v.x + ts.x * v.y + ns.x * v.z,
               ss.y * v.x + ts.y * v.y + ns.y * v.z,
               ss.z * v.x + ts.z * v.y + ns.z * v.z);
}

#endif// BSDF_BASE_GLSL
