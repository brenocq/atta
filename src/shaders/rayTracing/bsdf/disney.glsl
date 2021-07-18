//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disney.glsl
// Date: 2021-07-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BSDF_DISNEY_GLSL
#define BSDF_DISNEY_GLSL
#include "../base.glsl"
#include "../bxdf/base.glsl"
#include "base.glsl"
#include "../bxdf/disney.glsl"

BXDF MaterialToBXDF(Material material, vec2 uv)
{
	BXDF bxdf;
	bxdf.type = BXDF_TYPE_DISNEY;

	// TODO bump map
	bxdf.dataf[0] = material.datai[1]!=-1 ? texture(textures[material.datai[1]], uv).x : material.dataf[0];// metallic
	bxdf.dataf[1] = material.datai[2]!=-1 ? texture(textures[material.datai[2]], uv).x : material.dataf[1];// roughness
	bxdf.dataf[2] = material.datai[3]!=-1 ? texture(textures[material.datai[3]], uv).x : material.dataf[2];// subsurface
	bxdf.dataf[3] = material.datai[4]!=-1 ? texture(textures[material.datai[4]], uv).x : material.dataf[3];// specular
	bxdf.dataf[4] = material.datai[5]!=-1 ? texture(textures[material.datai[5]], uv).x : material.dataf[4];// specular tint
	bxdf.dataf[5] = material.datai[6]!=-1 ? texture(textures[material.datai[6]], uv).x : material.dataf[5];// eta
	bxdf.dataf[6] = material.datai[7]!=-1 ? texture(textures[material.datai[7]], uv).x : material.dataf[6];// anisotropic
	bxdf.dataf[7] = material.datai[8]!=-1 ? texture(textures[material.datai[8]], uv).x : material.dataf[7];// sheen
	bxdf.dataf[8] = material.datai[9]!=-1 ? texture(textures[material.datai[9]], uv).x : material.dataf[8];// sheen tint
	bxdf.dataf[9] = material.datai[10]!=-1 ? texture(textures[material.datai[10]], uv).x : material.dataf[9];// clear coat
	bxdf.dataf[10] = material.datai[11]!=-1 ? texture(textures[material.datai[11]], uv).x : material.dataf[10];// clear coat gloss
	bxdf.dataf[11] = material.datai[12]!=-1 ? texture(textures[material.datai[12]], uv).x : material.dataf[11];// transmission
	
	bxdf.datav[0] = material.datai[0]!=-1 ? texture(textures[material.datai[0]], uv).xyz : material.datav[0].xyz;// color

	return bxdf;
}

vec3 BSDF_f(BSDF bsdf, vec3 woW, vec3 wiW, uint flags)
{
	vec3 wi = BSDF_worldToLocal(bsdf, wiW), wo = BSDF_worldToLocal(bsdf, woW);
	if(wo.z == 0) return vec3(0,0,0);

	return BXDF_Disney_f(wo, wi, bsdf.bxdf);
}

vec3 BSDF_sampleF(BSDF bsdf, vec3 woW, out vec3 wiW, vec2 u, out float pdf, inout uint sampledType)
{
	// Sample BXDF
	vec3 wi, wo = BSDF_worldToLocal(bsdf, woW);
	pdf = 0.0f;
	sampledType = BXDF_FLAG_NONE;
	if(wo.z==0) return vec3(0,0,0);

	vec3 f = BXDF_Disney_sampleF(wo, wi, u, pdf, bsdf.bxdf);
	sampledType = BXDF_Disney_flags();

	wiW = BSDF_localToWorld(bsdf, wi);
	return f;
}

float BSDF_pdf(BSDF bsdf, vec3 woW, vec3 wiW, uint bxdfFlags)
{
	vec3 wo = BSDF_worldToLocal(bsdf, woW), wi = BSDF_worldToLocal(bsdf, wiW);
	if(wo.z == 0) return 0;
	float pdf = 0;

	return BXDF_Disney_pdf(wo, wi, bsdf.bxdf);
}

#endif// BSDF_DISNEY_GLSL
