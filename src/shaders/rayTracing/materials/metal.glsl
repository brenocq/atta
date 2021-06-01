//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// metal.glsl
// Date: 2021-02-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_METAL_GLSL
#define MATERIALS_METAL_GLSL
#include "base.glsl"
#include "../base.glsl"
#include "../bxdf/base.glsl"
#include "../bxdf/fresnel.glsl"
#include "../bxdf/microfacetDistribution.glsl"

BSDF Material_Metal_computeScatteringFunctions(inout Material material, vec2 uv)
{
	BXDF bxdf;
	bxdf.type = BXDF_TYPE_MICROFACET_REFLECTION;	
	bxdf.datav[0] = vec3(1,1,1);// R (not used)

	int etaTexIndex = material.datai[0];
	int kTexIndex = material.datai[1];
	int roughTexIndex = material.datai[2];
	int uroughTexIndex = material.datai[3];
	int vroughTexIndex = material.datai[4];
	int bumpTexIndex = material.datai[5];
	bool remapRoughness = material.datai[6]==1;

	// TODO bumpMap not supported yet
	
	float uRough;
	float vRough;

	//---------- Roughness ----------//
	uRough = uroughTexIndex==-1 ? material.dataf[1] : texture(textures[uroughTexIndex], uv).x;
	vRough = vroughTexIndex==-1 ? material.dataf[2] : texture(textures[vroughTexIndex], uv).x;
	// Substitute -1 by default roughness
	if(roughTexIndex!=-1 || material.dataf[0]!=-1)
	{
		float rough = roughTexIndex==-1 ? material.dataf[0] : texture(textures[roughTexIndex], uv).x;
		if(uRough == -1)
			uRough = rough;
		if(vRough == -1)
			vRough = rough;
	}

	if(remapRoughness)
	{
		uRough = BXDF_BXDF_MicrofacetDistribution_TrowbridgeReitz_roughnessToAlpha(uRough);
		vRough = BXDF_BXDF_MicrofacetDistribution_TrowbridgeReitz_roughnessToAlpha(vRough);
	}

	//---------- Roughness ----------//
	vec3 eta = etaTexIndex==-1 ? material.datav[0].xyz : texture(textures[etaTexIndex], uv).xyz;
	vec3 k = kTexIndex==-1 ? material.datav[1].xyz : texture(textures[kTexIndex], uv).xyz;

	//---------- Fresnel ----------//
	bxdf.datai[0] = BXDF_FRESNEL_TYPE_CONDUCTOR;// Fresnel type
	bxdf.datav[1] = vec3(1,1,1);// etaI
	bxdf.datav[2] = eta;// etaT
	bxdf.datav[3] = k;// k
	
	//---------- Microfacet Distribution ----------//
	bxdf.datai[1] = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;// Microfacet Distribution type
	bxdf.datai[2] = 1;// sampleVisibleArea
	bxdf.dataf[0] = uRough;// alphaX
	bxdf.dataf[1] = vRough;// alphaY

	BSDF bsdf;
	bsdf.bxdf[0] = bxdf;
	bsdf.nBxdf = 1;

	return bsdf;
}

#endif// MATERIALS_METAL_GLSL
