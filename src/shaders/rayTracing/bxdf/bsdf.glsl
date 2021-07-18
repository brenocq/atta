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
	return vec3(dot(v, bsdf.ss), dot(v, bsdf.ts), dot(v, bsdf.sn));
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

vec3 BSDF_worldToLocal(vec3 v)
{
	return vec3(dot(v, ray.it.bsdf.ss), dot(v, ray.it.bsdf.ts), dot(v, ray.it.bsdf.ns));
}

vec3 BSDF_localToWorld(vec3 v)
{
	vec3 ss = ray.it.bsdf.ss;
	vec3 ts = ray.it.bsdf.ts;
	vec3 ns = ray.it.bsdf.ns;
	return vec3(ss.x * v.x + ts.x * v.y + ns.x * v.z,
               ss.y * v.x + ts.y * v.y + ns.y * v.z,
               ss.z * v.x + ts.z * v.y + ns.z * v.z);
}

int BSDF_numComponents(BSDF bsdf, uint flags)
{
	int qty=0;
	for(int i=0; i<bsdf.nBxdf; i++)
		if(BXDF_matchesFlags(bsdf.bxdf[i], flags))
			qty++;

	return qty;
}

//---------- Main Methods ----------//
vec3 BSDF_f(vec3 woW, vec3 wiW, uint flags)
{
	vec3 wi = BSDF_worldToLocal(wiW), wo = BSDF_worldToLocal(woW);
	if(wo.z == 0) return vec3(0,0,0);

	bool reflection = dot(wiW, ray.it.bsdf.ng) * dot(woW, ray.it.bsdf.ng) > 0;

	vec3 f = vec3(0,0,0);

	//int nBxdf = ray.it.bsdf.nBxdf;
	for(int i=0; i<1; i++)
	{
		//uint bxdfFlags = BXDF_flags(ray.it.bsdf.bxdf[i].type);
		//if((bxdfFlags & flags) == bxdfFlags &&
		//	((reflection && (bxdfFlags & BXDF_FLAG_REFLECTION)!=0) ||
		//		(!reflection && (bxdfFlags & BXDF_FLAG_TRANSMISSION)!=0)))
		f += BXDF_f(ray.it.bsdf.bxdf[i], wo, wi);
	}

	return f;
}

vec3 BSDF_sampleF(BSDF bsdf, vec3 woW, out vec3 wiW, vec2 u, out float pdf, uint type, out uint sampledType)
{
	bsdf.nBxdf = 1;
	int matchingComps = BSDF_numComponents(bsdf, type);
	if(matchingComps == 0)
	{
		pdf = 0;
		sampledType = BXDF_FLAG_NONE;
		return vec3(0,0,0);
	}
	int comp = int(min(floor(u.x*matchingComps), matchingComps-1));
	BXDF bxdf;

	// Get bxdf with matching flags
	int count = comp;
	int selIndex;
	for(int i=0; i<bsdf.nBxdf; i++)
		if(BXDF_matchesFlags(bsdf.bxdf[i], type) && count--==0)
		{
			selIndex = i;
			bxdf = bsdf.bxdf[i];
			break;
		}

	// TODO remap

	// Sample BXDF
	vec3 wi, wo = BSDF_worldToLocal(bsdf, woW);
	if(wo.z==0) return vec3(0,0,0);
	pdf = 0.f;
	sampledType = BXDF_flags(bxdf);
	vec3 f = BXDF_sampleF(bxdf, wo, wi, u, pdf, sampledType);

	if(pdf == 0)
	{
		sampledType = BXDF_FLAG_NONE;
		return vec3(0,0,0);
	}
	wiW = BSDF_localToWorld(bsdf, wi);

	// Compute overall pdf with all matching BXDFs
	if((BXDF_flags(bxdf) & BXDF_FLAG_SPECULAR)==0 && matchingComps > 1)
		for(int i = 0; i < bsdf.nBxdf; i++)
			if(i != selIndex && BXDF_matchesFlags(bsdf.bxdf[i], type))
				pdf += BXDF_pdf(bsdf.bxdf[i], wo, wi);
	if(matchingComps > 1) pdf /= matchingComps;

	// Compute value of BSDF for sampled direction
	if((BXDF_flags(bxdf) & BXDF_FLAG_SPECULAR)==0)
	{
		bool reflection = dot(wiW, bsdf.ng) * dot(woW, bsdf.ng) > 0;
		f = vec3(0,0,0);
		for(int i = 0; i < bsdf.nBxdf; i++)
			if(BXDF_matchesFlags(bsdf.bxdf[i], type) &&
				((reflection && (BXDF_flags(bsdf.bxdf[i]) & BXDF_FLAG_REFLECTION)>0) ||
				 (!reflection && (BXDF_flags(bsdf.bxdf[i]) & BXDF_FLAG_TRANSMISSION)>0)))
				f += BXDF_f(bsdf.bxdf[i], wo, wi);
	}


	return f;
}

float BSDF_pdf(BSDF bsdf, vec3 woW, vec3 wiW, uint bxdfFlags)
{
	if(bsdf.nBxdf == 0) return 0;
	vec3 wo = BSDF_worldToLocal(bsdf, woW), wi = BSDF_worldToLocal(bsdf, wiW);
	if(wo.z == 0) return 0;
	float pdf = 0;

    int matchingComps = 0;
	for(int i = 0; i < bsdf.nBxdf; i++)
		if(BXDF_matchesFlags(bsdf.bxdf[i], bxdfFlags))
		{
			matchingComps++;
			pdf += BXDF_pdf(bsdf.bxdf[i], wo, wi);
		}
	float v = matchingComps > 0 ? pdf / matchingComps : 0.f;
	return v;
}

// Hemispherical-directional reflectance
vec3 BSDF_rho(BSDF bsdf, vec3 woW, int nSamples, vec2 samples[8], uint flags)
{
	//vec3 ret = vec3(0,0,0);
	//vec3 wo = BSDF_worldToLocal(bsdf, woW);
	//BXDF bxdf = bsdf.bxdf;
	//if(BXDF_matchesFlags(bxdf, flags))
	//	ret += BXDF_rho(bxdf, wo, nSamples, samples);
	//return ret;
	return vec3(0,0,0);
}

// Hemispherical-hemispherical reflectance
vec3 BSDF_rho(BSDF bsdf, int nSamples, vec2 samples0[8], vec2 samples1[8], uint flags)
{
	//vec3 ret = vec3(0,0,0);
	//BXDF bxdf = bsdf.bxdf;
	//if(BXDF_matchesFlags(bxdf, flags))
	//	ret += BXDF_rho(bxdf, nSamples, samples0, samples1);
	//return ret;
	return vec3(0,0,0);
}

#endif// BXDF_BSDF_GLSL
