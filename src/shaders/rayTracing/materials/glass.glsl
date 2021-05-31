//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// glass.glsl
// Date: 2021-06-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_GLASS_GLSL
#define MATERIALS_GLASS_GLSL
#include "base.glsl"
#include "../base.glsl"
#include "../bxdf/base.glsl"

//----- GLASS MATERIAL -----
// - datai
// 		[0](vec3 texture) Kr texture index
// 		[1](vec3 texture) Kt texture index
// 		[2](float texture) roughness texture index
// 		[3](float texture) roughnessu texture index
// 		[4](float texture) roughnessv texture index
// 		[5](float texture) eta texture index
// 		[6](float texture) bumpMap texture index
// 		[7](bool) active->remap urough and vrough from [0,1] to expected alpha
// - dataf
// 		[0] uniform roughness
// 		[1] uniform roughnessu
// 		[2] uniform roughnessv
// 		[3] uniform eta
// 		[4] uniform bumpMap
// - datav
// 		[0] uniform Kr
// 		[1] uniform Kt


BSDF Material_Glass_computeScatteringFunctions(inout Material material, vec2 uv)
{
	BXDF bxdf;
	bxdf.type = BXDF_TYPE_GLASS;

	// Kr
	bxdf.datav[0] = material.datai[0]==-1 ? material.datav[0].xyz : texture(textures[material.datai[0]], uv).xyz;
	// Kt
	bxdf.datav[1] = material.datai[1]==-1 ? material.datav[1].xyz : texture(textures[material.datai[1]], uv).xyz;
	// Eta
	bxdf.dataf[2] = material.datai[5]==-1 ? material.dataf[3] : texture(textures[material.datai[5]], uv).x;
	// Bump
	bxdf.dataf[3] = material.datai[6]==-1 ? material.dataf[4] : texture(textures[material.datai[6]], uv).x;

	//---------- Roughness ----------//
	float uRough;
	float vRough;
	if(material.datai[2]!=-1 || material.dataf[0]!=-1)// Isotropic roughness
	{
		float rough = material.datai[2]==-1 ? material.dataf[0] : texture(textures[material.datai[2]], uv).x;
		uRough = rough;
		vRough = rough;
	}
	else// Anisotropic roughness
	{
		uRough = material.datai[3]==-1 ? material.dataf[1] : texture(textures[material.datai[3]], uv).x;
		vRough = material.datai[4]==-1 ? material.dataf[2] : texture(textures[material.datai[4]], uv).x;
	}
	// Remap Roughness
	if(material.datai[7]==1)
	{
		uRough = BXDF_BXDF_MicrofacetDistribution_TrowbridgeReitz_roughnessToAlpha(uRough);
		vRough = BXDF_BXDF_MicrofacetDistribution_TrowbridgeReitz_roughnessToAlpha(vRough);
	}

	bxdf.dataf[0] = uRough;
	bxdf.dataf[1] = vRough;

	//---------- Create bsdf----------//
	BSDF bsdf;
	bsdf.bxdf[0] = bxdf;
	bsdf.nBxdf = 1;

	return bsdf;
}
#endif// MATERIALS_GLASS_GLSL
