//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// substrate.glsl
// Date: 2021-06-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_SUBSTRATE_GLSL
#define MATERIALS_SUBSTRATE_GLSL
#include "base.glsl"
#include "../base.glsl"
#include "../bxdf/base.glsl"

//----- SUBSTRATE MATERIAL -----
// - datai
// 		[0](vec3 texture) Kd texture index
// 		[1](vec3 texture) Ks texture index
// 		[2](float texture) roughness texture index
// 		[3](float texture) roughnessu texture index
// 		[4](float texture) roughnessv texture index
// 		[5](float texture) bumpMap texture index
// 		[6](bool) active->remap urough and vrough from [0,1] to expected alpha
// - dataf
// 		[0] uniform roughness
// 		[1] uniform roughnessu
// 		[2] uniform roughnessv
// 		[4] uniform bumpMap
// - datav
// 		[0] uniform Kd
// 		[1] uniform Ks


BSDF Material_Substrate_computeScatteringFunctions(inout Material material, vec2 uv)
{
	BXDF bxdf;
	bxdf.type = BXDF_TYPE_SUBSTRATE;

	// Kd
	bxdf.datav[0] = material.datai[0]==-1 ? material.datav[0].xyz : texture(textures[material.datai[0]], uv).xyz;
	// Ks
	bxdf.datav[1] = material.datai[1]==-1 ? material.datav[1].xyz : texture(textures[material.datai[1]], uv).xyz;
	// Bump
	bxdf.dataf[2] = material.datai[6]==-1 ? material.dataf[3] : texture(textures[material.datai[6]], uv).x;

	//---------- Roughness ----------//
	float uRough = material.datai[3]==-1 ? material.dataf[1] : texture(textures[material.datai[3]], uv).x;
	float vRough = material.datai[4]==-1 ? material.dataf[2] : texture(textures[material.datai[4]], uv).x;
	// Substitute -1 by default roughness
	if(material.datai[2]!=-1 || material.dataf[0]!=-1)// Isotropic roughness
	{
		float rough = material.datai[2]==-1 ? material.dataf[0] : texture(textures[material.datai[2]], uv).x;
		if(uRough==-1)
			uRough = rough;
		if(vRough==-1)
			vRough = rough;
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
#endif// MATERIALS_SUBSTRATE_GLSL
