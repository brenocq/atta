//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disney.glsl
// Date: 2021-02-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_GLSL
#define BXDF_DISNEY_GLSL
#include "../base.glsl"
#include "base.glsl"
#include "disneyClearcoat.glsl"
// - datai
// 		[0](bool) thin
// - dataf
// 		[0] metallic
// 		[1] eta
// 		[2] roughness
// 		[3] specularTint
// 		[4] anisotropic
// 		[5] sheen
// 		[6] sheenTint
// 		[7] clearCoat
// 		[8] clearCoatGloss
// 		[9] specTrans
// 		[10] flatness
// 		[11] diffTrans
// - datav
// 		[0] color
// 		[1] scatterDistance

void BXDF_Disney_preprocess(BXDF bxdf, 
		out bool microfacetReflection, out Fresnel frMR, out MicrofacetDistribution distMR,
		out bool clearCoat, out float cc, out float ccGloss)
{
	vec3 color = bxdf.datav[0];
	float metallicWeight = bxdf.dataf[0];
	float e = bxdf.dataf[1];
	float strans = bxdf.dataf[9];
	float diffuseWeight = (1-metallicWeight)*(1-strans);
	float dt = bxdf.dataf[11]/2;// 0->all difuse is reflected, 1->transmitted
	float rough = bxdf.dataf[2];
	float lum = lum_y(color);
	// Normalize lum to isolate hue+sat
	vec3 colorTint = lum>0 ? (color/lum) : vec3(1,1,1);

	float sheenWeight = bxdf.dataf[5];
	vec3 colorSheen;
	if(sheenWeight>0)
	{
		float sTint = bxdf.dataf[6];
		colorSheen = mix(vec3(1,1,1), colorTint, sTint);
	}

	if(diffuseWeight>0)
	{
		// TODO diffuse material
	}

	// Create the microfacet distribution for metallic and/or specular transmission
	float aspect = sqrt(1-bxdf.dataf[4]*.9);
	float ax = max(0.001f, sqr(rough)/aspect);
	float ay = max(0.001f, sqr(rough)*aspect);

	//---------- Microfacet Reflection ----------//
	microfacetReflection = true;
	distMR.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_DISNEY;
	distMR.alphaX = ax;
	distMR.alphaY = ay;
	distMR.sampleVisibleArea = 1;

	// Specular is Trowbridge-Reitz with a modified Fresnel function
	float specTint = bxdf.dataf[3];
	vec3 colorSpec0 = mix(schlickR0FromEta(e)*mix(vec3(1,1,1), colorTint, specTint), color, metallicWeight);

	frMR.type = BXDF_FRESNEL_TYPE_DISNEY;
	frMR.data0 = colorSpec0;
	frMR.data1.x = e;
	frMR.data1.y = metallicWeight;

	cc = bxdf.dataf[7];
	if(cc>0)
	{
		clearCoat = true;
		ccGloss = mix(.1, .001, bxdf.dataf[8]);
	}
}

//-------------------- F --------------------//
vec3 BXDF_Disney_f(vec3 wo, vec3 wi, BXDF bxdf)
{
	bool microfacetReflection; Fresnel frMR; MicrofacetDistribution distMR;
	bool clearCoat; float cc; float ccGloss;
	BXDF_Disney_preprocess(bxdf, 
			microfacetReflection, frMR, distMR,
			clearCoat, cc, ccGloss);

	vec3 f = vec3(0,0,0);
	if(microfacetReflection)
		f += BXDF_MicrofacetReflection_f(wo, wi, vec3(1,1,1), frMR, distMR);
	//if(clearCoat)
	//	f += BXDF_DisneyClearcoat_f(wo, wi, cc, ccGloss);

	return f;
}

vec3 BXDF_Disney_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, BXDF bxdf)
{
	bool microfacetReflection; Fresnel frMR; MicrofacetDistribution distMR;
	bool clearCoat; float cc; float ccGloss;
	BXDF_Disney_preprocess(bxdf, 
			microfacetReflection, frMR, distMR,
			clearCoat, cc, ccGloss);

	vec3 f = vec3(0,0,0);
	if(microfacetReflection)
		f += BXDF_MicrofacetReflection_sampleF(wo, wi, u, pdf, vec3(1,1,1), frMR, distMR);
	//if(clearCoat)
	//	f += BXDF_DisneyClearcoat_sampleF(wo, wi, u, pdf, cc, ccGloss);

	return f;
}

uint BXDF_Disney_flags()
{
	return BXDF_FLAG_ALL;
}

#endif// BXDF_DISNEY_GLSL
