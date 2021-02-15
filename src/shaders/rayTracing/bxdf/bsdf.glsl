//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// bsdf.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_BSDF_GLSL
#define BXDF_BSDF_GLSL
#include "base.glsl"
#include "bxdf.glsl"

//---------- Helpers ---------- //
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

//---------- Main Methods ----------//
vec3 BSDF_f(BSDF bsdf, vec3 woW, vec3 wiW, uint flags)
{
	vec3 wi = BSDF_worldToLocal(bsdf, wiW), wo = BSDF_worldToLocal(bsdf, woW);
	if(wo.z == 0) return vec3(0,0,0);

	bool reflection = dot(wiW, bsdf.ng) * dot(woW, bsdf.ng) > 0;

	vec3 f = vec3(0,0,0);

	BXDF bxdf = bsdf.bxdf;
	uint bxdfFlags = BXDF_flags(bxdf);
	if(BXDF_matchesFlags(bxdf, flags) &&
		((reflection && (bxdfFlags & BXDF_FLAG_REFLECTION)!=0) ||
			(!reflection && (bxdfFlags & BXDF_FLAG_TRANSMISSION)!=0)))
		f += BXDF_f(bxdf, wo, wi);

	return f;
}

vec3 BSDF_sampleF(BSDF bsdf, vec3 woW, out vec3 wiW, vec2 u, out float pdf, uint type, out uint sampledType)
{
	BXDF bxdf = bsdf.bxdf;
	if(!BXDF_matchesFlags(bxdf, type))
	{
		sampledType = 0;
		pdf = 0;
		return vec3(0,0,0);
	}

	// Sample BXDF
	vec3 wi, wo = BSDF_worldToLocal(bsdf, woW);
	pdf = 0.f;

	sampledType = BXDF_flags(bxdf);
	vec3 f = BXDF_sampleF(bxdf, wo, wi, u, pdf, sampledType);

	if(pdf == 0)
	{
		sampledType = 0;
		return vec3(0,0,0);
	}
	wiW = BSDF_localToWorld(bsdf, wi);

	return f;
}

float BSDF_pdf(vec3 wo, vec3 wi, uint bxdfFlags)
{
	// TODO
	return 0.f;
}

// Hemispherical-directional reflectance
vec3 BSDF_rho(BSDF bsdf, vec3 woW, int nSamples, vec2 samples[8], uint flags)
{
	vec3 ret = vec3(0,0,0);
	vec3 wo = BSDF_worldToLocal(bsdf, woW);
	BXDF bxdf = bsdf.bxdf;
	if(BXDF_matchesFlags(bxdf, flags))
		ret += BXDF_rho(bxdf, wo, nSamples, samples);
	return ret;
}

// Hemispherical-hemispherical reflectance
vec3 BSDF_rho(BSDF bsdf, int nSamples, vec2 samples0[8], vec2 samples1[8], uint flags)
{
	vec3 ret = vec3(0,0,0);
	BXDF bxdf = bsdf.bxdf;
	if(BXDF_matchesFlags(bxdf, flags))
		ret += BXDF_rho(bxdf, nSamples, samples0, samples1);
	return ret;
}

#endif// BXDF_BSDF_GLSL
