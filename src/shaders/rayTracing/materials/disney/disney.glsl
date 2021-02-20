//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disney.glsl
// Date: 2021-02-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_DISNEY_GLSL
#define MATERIALS_DISNEY_GLSL
#include "../base.glsl"
#include "../../base.glsl"
#include "../../bxdf/base.glsl"

BSDF Material_Disney_computeScatteringFunctions(inout Material material, vec2 uv)
{
	BXDF bxdf;
	bxdf.type = BXDF_TYPE_DISNEY;

	// TODO bump map

	bxdf.datai[0] = material.datai[12];// thin

	bxdf.dataf[0] = material.datai[1]!=-1 ? texture(textures[material.datai[1]], uv).x : material.dataf[0];// metallic
	bxdf.dataf[1] = material.datai[2]!=-1 ? texture(textures[material.datai[2]], uv).x : material.dataf[1];// eta
	bxdf.dataf[2] = material.datai[3]!=-1 ? texture(textures[material.datai[3]], uv).x : material.dataf[2];// roughness
	bxdf.dataf[3] = material.datai[4]!=-1 ? texture(textures[material.datai[4]], uv).x : material.dataf[3];// specularTint
	bxdf.dataf[4] = material.datai[5]!=-1 ? texture(textures[material.datai[5]], uv).x : material.dataf[4];// anisotropic
	bxdf.dataf[5] = material.datai[6]!=-1 ? texture(textures[material.datai[6]], uv).x : material.dataf[5];// sheen
	bxdf.dataf[6] = material.datai[7]!=-1 ? texture(textures[material.datai[7]], uv).x : material.dataf[6];// sheenTint
	bxdf.dataf[7] = material.datai[8]!=-1 ? texture(textures[material.datai[8]], uv).x : material.dataf[7];// clearCoat
	bxdf.dataf[8] = material.datai[9]!=-1 ? texture(textures[material.datai[9]], uv).x : material.dataf[8];// clearCoatGloss
	bxdf.dataf[9] = material.datai[10]!=-1 ? texture(textures[material.datai[10]], uv).x : material.dataf[9];// specTrans
	bxdf.dataf[10] = material.datai[13]!=-1 ? texture(textures[material.datai[13]], uv).x : material.dataf[10];// flatness
	bxdf.dataf[11] = material.datai[14]!=-1 ? texture(textures[material.datai[14]], uv).x : material.dataf[11];// diffTrans
	
	bxdf.datav[0] = material.datai[0]!=-1 ? texture(textures[material.datai[0]], uv).xyz : material.datav[0].xyz;// color
	bxdf.datav[1] = material.datai[10]!=-1 ? texture(textures[material.datai[10]], uv).xyz : material.datav[1].xyz;// scatterDistance

	BSDF bsdf;
	bsdf.nBxdf = 1;
	bsdf.bxdf[0] = bxdf;
	return bsdf;
}

#endif// MATERIALS_DISNEY_GLSL
