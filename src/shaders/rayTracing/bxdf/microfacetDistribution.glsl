//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// microfacetDistribution.glsl
// Date: 2021-02-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_MICROFACET_DISTRIBUTION_GLSL
#define BXDF_MICROFACET_DISTRIBUTION_GLSL
#include "base.glsl"
#include "../base.glsl"
#include "microfacetDistributionHelpers.glsl"

const uint BXDF_MICROFACET_DISTRIBUTION_TYPE_NONE 				= 0;
const uint BXDF_MICROFACET_DISTRIBUTION_TYPE_BECKMANN 			= 1;
const uint BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ	= 2;
const uint BXDF_MICROFACET_DISTRIBUTION_TYPE_DISNEY				= 3;

struct MicrofacetDistribution
{
	uint type;
	float alphaX;
	float alphaY;
	uint sampleVisibleArea;
};

float BXDF_MicrofacetDistribution_Beckmann_D(MicrofacetDistribution dist, vec3 wh);
float BXDF_MicrofacetDistribution_Beckmann_lambda(MicrofacetDistribution dist, vec3 w);
vec3 BXDF_MicrofacetDistribution_Beckmann_sampleWh(MicrofacetDistribution dist, vec3 wo, vec2 u);
float BXDF_MicrofacetDistribution_Beckmann_pdf(MicrofacetDistribution dist, vec3 wo, vec3 wh);
float BXDF_MicrofacetDistribution_TrowbridgeReitz_D(MicrofacetDistribution dist, vec3 wh);
float BXDF_MicrofacetDistribution_TrowbridgeReitz_lambda(MicrofacetDistribution dist, vec3 w);
vec3 BXDF_MicrofacetDistribution_TrowbridgeReitz_sampleWh(MicrofacetDistribution dist, vec3 wo, vec2 u);
float BXDF_MicrofacetDistribution_TrowbridgeReitz_pdf(MicrofacetDistribution dist, vec3 wo, vec3 wh);

float BXDF_MicrofacetDistribution_D(MicrofacetDistribution dist, vec3 wh)
{
	switch(dist.type)
	{
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_BECKMANN:
			return BXDF_MicrofacetDistribution_Beckmann_D(dist, wh);
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ:
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_DISNEY:
			return BXDF_MicrofacetDistribution_TrowbridgeReitz_D(dist, wh);
		default:
			return 0;
	}
}

float BXDF_MicrofacetDistribution_lambda(MicrofacetDistribution dist, vec3 w)
{
	switch(dist.type)
	{
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_BECKMANN:
			return BXDF_MicrofacetDistribution_Beckmann_lambda(dist, w);
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ:
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_DISNEY:
			return BXDF_MicrofacetDistribution_TrowbridgeReitz_lambda(dist, w);
		default:
			return 0;
	}
}

float BXDF_MicrofacetDistribution_G(MicrofacetDistribution dist, vec3 wo, vec3 wi)
{
	if(dist.type == BXDF_MICROFACET_DISTRIBUTION_TYPE_DISNEY)
	{
		float G1Wo = 1/(1+BXDF_MicrofacetDistribution_lambda(dist, wo));
		float G1Wi = 1/(1+BXDF_MicrofacetDistribution_lambda(dist, wi));
		return G1Wo*G1Wi;
	}
	else
	{
		float lambdaWo = BXDF_MicrofacetDistribution_lambda(dist, wo);
		float lambdaWi = BXDF_MicrofacetDistribution_lambda(dist, wi);
		return 1/(1+lambdaWo+lambdaWi);
	}
}

vec3 BXDF_MicrofacetDistribution_sampleWh(MicrofacetDistribution dist, vec3 wo, vec2 u)
{
	switch(dist.type)
	{
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_BECKMANN:
			return BXDF_MicrofacetDistribution_Beckmann_sampleWh(dist, wo, u);
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ:
		case BXDF_MICROFACET_DISTRIBUTION_TYPE_DISNEY:
			return BXDF_MicrofacetDistribution_TrowbridgeReitz_sampleWh(dist, wo, u);
		default:
			return vec3(0,0,0);
	}
}

float BXDF_MicrofacetDistribution_pdf(MicrofacetDistribution dist, vec3 wo, vec3 wh)
{
	float D = BXDF_MicrofacetDistribution_D(dist, wh);

	if(dist.sampleVisibleArea!=0)
	{
		float G1 = 1.0f/(1+BXDF_MicrofacetDistribution_lambda(dist, wo));
		return D * G1 * abs(dot(wo, wh)) / absCosTheta(wo);
	}
	else
		return D * absCosTheta(wh);
}

//-------------------- Beckmann --------------------//
float BXDF_MicrofacetDistribution_Beckmann_D(MicrofacetDistribution dist, vec3 wh)
{
	float alphaX = dist.alphaX;
	float alphaY = dist.alphaY;

	float tan2Th = tan2Theta(wh);
	if(isinf(tan2Th)) return 0;
	float cos4Th = cos2Theta(wh)*cos2Theta(wh);
	return exp(-tan2Th * (cos2Phi(wh)/(alphaX*alphaX)+
						  sin2Phi(wh)/(alphaY*alphaY)))/
							(pi * alphaX * alphaY * cos4Th);
}

float BXDF_MicrofacetDistribution_Beckmann_lambda(MicrofacetDistribution dist, vec3 w)
{
	float absTanTheta = abs(tanTheta(w));
	if(isinf(absTanTheta)) return 0.;
	// Compute alpha for direction w
	float alpha = sqrt(cos2Phi(w) * dist.alphaX * dist.alphaX + sin2Phi(w) * dist.alphaY * dist.alphaY);
	float a = 1 / (alpha * absTanTheta);
	if(a >= 1.6f) return 0;
	return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

vec3 BXDF_MicrofacetDistribution_Beckmann_sampleWh(MicrofacetDistribution dist, vec3 wo, vec2 u)
{
	if(dist.sampleVisibleArea == 0)
	{
		// Sample full distribution of normals for Beckmann distribution

		// Compute tan^2 theta and phi for Beckmann distribution sample
		float tan2Th, phi;
		if(dist.alphaX == dist.alphaY)
		{
			float logSample = log(1 - u.x); 
			//DCHECK(!isinf(logSample)); 
			tan2Th = -dist.alphaX * dist.alphaX * logSample; 
			phi = u.y * 2 * pi;            
		}
		else
		{           
			// Compute _tan2Theta_ and _phi_ for anisotropic Beckmann
			// distribution
			float logSample = log(1 - u.x); 
			//DCHECK(!std::isinf(logSample)); 
			phi = atan(dist.alphaY / dist.alphaX *
							tan(2 * pi * u.y + 0.5f * pi));
			if(u.y > 0.5f) phi += pi;
			float sinPh = sin(phi), cosPh = cos(phi);
			float alphax2 = dist.alphaX * dist.alphaX, alphay2 = dist.alphaY * dist.alphaY;
			tan2Th = -logSample / (cosPh * cosPh / alphax2 + sinPh * sinPh / alphay2);
		}

		// Map sampled Beckmann angles to normal direction _wh_
		float cosTh = 1 / sqrt(1 + tan2Th); 
		float sinTh = sqrt(max(0.0f, 1 - cosTh*cosTh));
		return vec3(0,0,0); // TODO finish implementation
		vec3 wh = sphericalDirection(sinTh, cosTh, phi);
		if(!sameHemisphere(wo, wh)) wh = -wh;
		return wh;         
	}
	else
	{               
		// Sample visible area of normals for Beckmann distribution
		vec3 wh;       
		bool flip = wo.z < 0;
		wh = beckmannSample(flip ? -wo : wo, dist.alphaX, dist.alphaY, u.x, u.y);
		if(flip) wh = -wh;
		return wh;         
	}
}

//-------------------- Trowbridge-Reitz --------------------//
float BXDF_MicrofacetDistribution_TrowbridgeReitz_D(MicrofacetDistribution dist, vec3 wh)
{
	float alphaX = dist.alphaX;
	float alphaY = dist.alphaY;

	float tan2Th = tan2Theta(wh);
	if(isinf(tan2Th)) return 0;
	float cos4Th = cos2Theta(wh)*cos2Theta(wh);
	float e = (cos2Phi(wh)/(alphaX*alphaX)+sin2Phi(wh)/(alphaY*alphaY))*tan2Th;
	return 1/(pi*alphaX*alphaY*cos4Th*(1+e)*(1+e));
}

float BXDF_MicrofacetDistribution_TrowbridgeReitz_lambda(MicrofacetDistribution dist, vec3 w)
{
	float absTanTheta = abs(tanTheta(w));
	if(isinf(absTanTheta)) return 0.;
	// Compute alpha for direction w
	float alpha = sqrt(cos2Phi(w) * dist.alphaX * dist.alphaX + sin2Phi(w) * dist.alphaY * dist.alphaY);
	float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
	return (-1 + sqrt(1.f + alpha2Tan2Theta)) / 2;
}

vec3 BXDF_MicrofacetDistribution_TrowbridgeReitz_sampleWh(MicrofacetDistribution dist, vec3 wo, vec2 u)
{
	vec3 wh;
	float alphaX = dist.alphaX;
	float alphaY = dist.alphaY;
	if(dist.sampleVisibleArea==0)
	{
		float cosTh = 0, phi = (2 * pi) * u.y;
		if(alphaX == alphaY)
		{
			float tanTheta2 = alphaX * alphaX * u.x / (1.0f - u.x);
			cosTh = 1 / sqrt(1 + tanTheta2);
		}
		else
		{
			phi = atan(alphaY / alphaX * tan(2 * pi * u.y + .5f * pi));
			if(u.y > .5f) phi += pi;
			float sinPh = sin(phi), cosPh = cos(phi);
			const float alphax2 = alphaX * alphaX, alphay2 = alphaY * alphaY;
			const float alpha2 = 1 / (cosPh * cosPh / alphax2 + sinPh * sinPh / alphay2);
			float tanTheta2 = alpha2 * u.x / (1 - u.x);
			cosTh = 1 / sqrt(1 + tanTheta2);
		}
		float sinTh = sqrt(max(0.0f, 1.0f - cosTh * cosTh));
		wh = sphericalDirection(sinTh, cosTh, phi);
		if(!sameHemisphere(wo, wh)) wh = -wh;
	} else {
		bool flip = wo.z < 0;
		wh = trowbridgeReitzSample(flip ? -wo : wo, alphaX, alphaY, u.x, u.y);
		if(flip) wh = -wh;
	}
	return wh;
}

float BXDF_BXDF_MicrofacetDistribution_TrowbridgeReitz_roughnessToAlpha(float roughness)
{
	roughness = max(roughness, 0.001f);
	float x = log(roughness);  
	return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
		   0.000640711f * x * x * x * x;  
}

#endif// BXDF_MICROFACET_DISTRIBUTION_GLSL
