//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disney.glsl
// Date: 2021-02-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_GLSL
#define BXDF_DISNEY_GLSL
#include "../base.glsl"
#include "disneyClearcoat.glsl"
#include "disneyRetro.glsl"
#include "disneyDiffuse.glsl"
#include "disneySheen.glsl"
#include "disneyFakeSS.glsl"
#include "../../samplers/random.glsl"
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
		out bool clearCoat, out float cc, out float ccGloss,
		out bool microfacetTransmission, out MicrofacetDistribution distMT, out vec3 T, out float etaA, out float etaB,
		out bool lambertianTransmission, out vec3 Tl,
		out bool disneyRetro, out vec3 Rr, out float roughRetro,
		out bool disneyDiffuse, out vec3 Rd,
		out bool disneySheen, out vec3 Rs,
		out bool disneyFakeSS, out vec3 Rf, out float roughFake)
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
	bool thin = bxdf.datai[0]==1;

	float sheenWeight = bxdf.dataf[5];
	vec3 colorSheen;
	if(sheenWeight>0)
	{
		float sTint = bxdf.dataf[6];
		colorSheen = mix(vec3(1,1,1), colorTint, sTint);
	}

	disneyRetro = false;
	disneyDiffuse = false;
	disneySheen = false;
	disneyFakeSS = false;
	if(diffuseWeight>0)
	{
		if(thin)
		{
			//---------- Diffuse ----------//
			float flatt = bxdf.dataf[10];
			disneyDiffuse = true;
			Rd = diffuseWeight*(1-flatt)*(1-dt)*color;

			//---------- FakeSS ----------//
			disneyFakeSS = true;
			Rf = diffuseWeight*flatt*(1 - dt)*color;
			roughFake = rough;
		}
		else
		{
			vec3 sd = bxdf.datav[1];
			if(isBlack(sd))
			{
				//---------- Diffuse ----------//
				disneyDiffuse = true;
				Rd = diffuseWeight*color;
			}
			else
			{
				// BSSRDF
			}
		}

		//---------- Retro-reflection ----------//
		disneyRetro = true;
		Rr = color*diffuseWeight;
		roughRetro = rough;

		//---------- Sheen ----------//
		if(sheenWeight>0)
		{
			disneySheen = true;
			Rs = diffuseWeight*sheenWeight*colorSheen;
		}
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

	//---------- Clearcoat ----------//
	cc = bxdf.dataf[7];
	if(cc>0)
	{
		clearCoat = true;
		ccGloss = mix(.1, .001, bxdf.dataf[8]);
	}

	//---------- Microfacet Transmission ----------//
	if(strans > 0)
	{
		microfacetTransmission = true;
		T = strans * sqrt(color);    
		etaA = 1.0;
		etaB = e;
		if(thin)
		{        
			// Scale roughness based on IOR (Burley 2015, Figure 15).
			float rscaled = (0.65f * e - 0.35f) * rough;
			float ax = max(0.001f, sqr(rscaled) / aspect);
			float ay = max(0.001f, sqr(rscaled) * aspect); 
			distMT.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ; 
			distMT.alphaX = ax;
			distMT.alphaY = ay;
			distMT.sampleVisibleArea = 1;
		}
		else
		{
			distMT = distMR;
		}
	}
	else
		microfacetTransmission = false;

	//---------- Lambertian Transmission ----------//
	if(thin)
	{
		lambertianTransmission = true;
		Tl = dt*color;
	}
	else 
		lambertianTransmission = false;

}

//-------------------- F --------------------//
vec3 BXDF_Disney_f(vec3 wo, vec3 wi, BXDF bxdf)
{
	bool microfacetReflection; Fresnel frMR; MicrofacetDistribution distMR;
	bool clearCoat; float cc; float ccGloss;
	bool microfacetTransmission; MicrofacetDistribution distMT; vec3 T; float etaA; float etaB;
	bool lambertianTransmission; vec3 Tl;
	bool disneyRetro; vec3 Rr; float roughnessRetro;
	bool disneyDiffuse; vec3 Rd;
	bool disneySheen; vec3 Rs;
	bool disneyFakeSS; vec3 Rf; float roughnessFakeSS;
	BXDF_Disney_preprocess(bxdf, 
			microfacetReflection, frMR, distMR,
			clearCoat, cc, ccGloss,
			microfacetTransmission, distMT, T, etaA, etaB,
			lambertianTransmission, Tl,
			disneyRetro, Rr, roughnessRetro,
			disneyDiffuse, Rd,
			disneySheen, Rs,
			disneyFakeSS, Rf, roughnessFakeSS);

	vec3 f = vec3(0,0,0);
	if(microfacetReflection)
		f += BXDF_MicrofacetReflection_f(wo, wi, vec3(1,1,1), frMR, distMR);
	if(microfacetTransmission)
		f += BXDF_MicrofacetTransmission_f(wo, wi, T, etaA, etaB, distMT);
	if(clearCoat)
		f += BXDF_DisneyClearcoat_f(wo, wi, cc, ccGloss);
	if(lambertianTransmission)
		f += BXDF_LambertianTransmission_f(Tl);
	if(disneyRetro)
		f += BXDF_DisneyRetro_f(wo, wi, Rr, roughnessRetro);
	if(disneyDiffuse)
		f += BXDF_DisneyDiffuse_f(wo, wi, Rd);
	if(disneySheen)
		f += BXDF_DisneySheen_f(wo, wi, Rs);
	if(disneyFakeSS)
		f += BXDF_DisneyFakeSS_f(wo, wi, Rf, roughnessFakeSS);

	return f;
}

vec3 BXDF_Disney_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, BXDF bxdf)
{
	bool microfacetReflection; Fresnel frMR; MicrofacetDistribution distMR;
	bool clearCoat; float cc; float ccGloss;
	bool microfacetTransmission; MicrofacetDistribution distMT; vec3 T; float etaA; float etaB;
	bool lambertianTransmission; vec3 Tl;
	bool disneyRetro; vec3 Rr; float roughnessRetro;
	bool disneyDiffuse; vec3 Rd;
	bool disneySheen; vec3 Rs;
	bool disneyFakeSS; vec3 Rf; float roughnessFakeSS;
	BXDF_Disney_preprocess(bxdf, 
			microfacetReflection, frMR, distMR,
			clearCoat, cc, ccGloss,
			microfacetTransmission, distMT, T, etaA, etaB,
			lambertianTransmission, Tl,
			disneyRetro, Rr, roughnessRetro,
			disneyDiffuse, Rd,
			disneySheen, Rs,
			disneyFakeSS, Rf, roughnessFakeSS);

	vec3 f = vec3(0,0,0);
	uint seed  = uint(u.x*10000);
	float random = randomFloat(seed);

	// Sample one BXDF
	int qtyBXDF = int(microfacetReflection)+int(microfacetTransmission)+
		int(clearCoat)+int(lambertianTransmission)+int(disneyRetro)+
		int(disneyDiffuse)+int(disneySheen)+int(disneyFakeSS);
	int count = min(int(random*qtyBXDF), qtyBXDF-1);
	if(microfacetReflection && count--==0)
		f += BXDF_MicrofacetReflection_sampleF(wo, wi, u, pdf, vec3(1,1,1), frMR, distMR);
	if(microfacetTransmission && count--==0)
		f += BXDF_MicrofacetTransmission_sampleF(wo, wi, u, pdf, T, etaA, etaB, distMT);
	if(clearCoat && count--==0)
		f += BXDF_DisneyClearcoat_sampleF(wo, wi, u, pdf, cc, ccGloss);
	if(lambertianTransmission && count--==0)
		f += BXDF_LambertianTransmission_sampleF(wo, wi, u, pdf, Tl);
	if(disneyRetro && count--==0)
		f += BXDF_DisneyRetro_sampleF(wo, wi, u, pdf, Rr, roughnessRetro);
	if(disneyDiffuse && count--==0)
		f += BXDF_DisneyDiffuse_sampleF(wo, wi, u, pdf, Rd);
	if(disneySheen && count--==0)
		f += BXDF_DisneySheen_sampleF(wo, wi, u, pdf, Rs);
	if(disneyFakeSS && count--==0)
		f += BXDF_DisneyFakeSS_sampleF(wo, wi, u, pdf, Rf, roughnessFakeSS);

	return f;
}

uint BXDF_Disney_flags()
{
	return BXDF_FLAG_ALL;
}

#endif// BXDF_DISNEY_GLSL
