//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// diffuse.glsl
// Date: 2021-07-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BSDF_DIFFUSE_GLSL
#define BSDF_DIFFUSE_GLSL
#include "../base.glsl"
#include "../bxdf/base.glsl"
#include "../bxdf/lambertianReflection.glsl"
#include "../bxdf/orenNayar.glsl"
#include "base.glsl"

BXDF MaterialToBXDF(Material material, vec2 uv)
{
	BXDF bxdf;

	vec3 Kd = material.datai[0]==-1 ? material.datav[0].xyz : texture(textures[material.datai[0]], uv).xyz;
	float sigma = material.datai[1]==-1 ? material.dataf[0] : texture(textures[material.datai[1]], uv).x;

	if(!isBlack(Kd))// If not black
	{
		if(sigma == 0)
		{
			// Add Lambertian
			bxdf.type = BXDF_TYPE_LAMBERTIAN_REFLECTION;	
			bxdf.datav[0] = Kd;// R
		}
		else
		{
			// Add Oren Nayar
			bxdf.type = BXDF_TYPE_OREN_NAYAR;
			bxdf.datav[0] = Kd;// R
			bxdf.dataf[0] = sigma;// Sigma
		}
	}
	return bxdf;
}

vec3 BSDF_f(BSDF bsdf, vec3 woW, vec3 wiW, uint flags)
{
	vec3 wi = BSDF_worldToLocal(bsdf, wiW), wo = BSDF_worldToLocal(bsdf, woW);
	if(wo.z == 0) return vec3(0,0,0);

	if(bsdf.bxdf.type == BXDF_TYPE_OREN_NAYAR)
		return BXDF_OrenNayar_f(wo, wi, bsdf.bxdf.datav[0], bsdf.bxdf.dataf[0]);
	else
		return BXDF_LambertianReflection_f(bsdf.bxdf.datav[0]);
}

vec3 BSDF_sampleF(BSDF bsdf, vec3 woW, out vec3 wiW, vec2 u, out float pdf, inout uint sampledType)
{
	// Sample BXDF
	vec3 wi, wo = BSDF_worldToLocal(bsdf, woW);
	pdf = 0.0f;
	sampledType = BXDF_FLAG_NONE;
	if(wo.z==0) return vec3(0,0,0);

	vec3 f;
	if(bsdf.bxdf.type == BXDF_TYPE_OREN_NAYAR)
	{
		f = BXDF_OrenNayar_sampleF(wo, wi, u, pdf, bsdf.bxdf.datav[0], bsdf.bxdf.dataf[0]);
		sampledType = BXDF_OrenNayar_flags();
	}
	else
	{
		f = BXDF_LambertianReflection_sampleF(wo, wi, u, pdf, bsdf.bxdf.datav[0]);
		sampledType = BXDF_LambertianReflection_flags();
	}

	wiW = BSDF_localToWorld(bsdf, wi);
	return f;
}

float BSDF_pdf(BSDF bsdf, vec3 woW, vec3 wiW, uint bxdfFlags)
{
	vec3 wo = BSDF_worldToLocal(bsdf, woW), wi = BSDF_worldToLocal(bsdf, wiW);
	if(wo.z == 0) return 0;
	float pdf = 0;

	if(bsdf.bxdf.type == BXDF_TYPE_OREN_NAYAR)
		return BXDF_OrenNayar_pdf(wo, wi);
	else
		return BXDF_LambertianReflection_pdf(wo, wi);
}

#endif// BSDF_DIFFUSE_GLSL
