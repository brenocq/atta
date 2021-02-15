//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// fresnel.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_FRESNEL_GLSL
#define BXDF_FRESNEL_GLSL
#include "base.glsl"

uint BXDF_FRESNEL_TYPE_DIELETRIC = 0;
uint BXDF_FRESNEL_TYPE_CONDUCTOR = 1;
uint BXDF_FRESNEL_TYPE_NOOP 	 = 2;

struct Fresnel
{
	uint type;

	// Dieletric -> Only use etaI[0] and etaT[0] for the coefficients of refraction
	// Conductor -> Use all variables
	vec3 etaI;
	vec3 etaT;
	vec3 k;
};

vec3 BXDF_Fresnel_evaluate(Fresnel fresnel, float cosThetaI)
{
	if(fresnel.type == BXDF_FRESNEL_TYPE_DIELETRIC)
	{
		float Fr = FrDieletric(cosThetaI, fresnel.etaI.x, fresnel.etaT.x);
		return vec3(Fr, Fr, Fr);
	}
	else if(fresnel.type == BXDF_FRESNEL_TYPE_CONDUCTOR)
	{
		return FrConductor(cosThetaI, fresnel.etaI, fresnel.etaT, fresnel.k);
	}
	else
	{
		return vec3(1,1,1);
	}
}

#endif// BXDF_FRESNEL_GLSL
