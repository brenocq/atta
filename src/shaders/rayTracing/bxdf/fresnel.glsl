//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// fresnel.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_FRESNEL_GLSL
#define BXDF_FRESNEL_GLSL
#include "base.glsl"

const uint BXDF_FRESNEL_TYPE_DIELETRIC 	= 0;
const uint BXDF_FRESNEL_TYPE_CONDUCTOR 	= 1;
const uint BXDF_FRESNEL_TYPE_NOOP 	 	= 2;
const uint BXDF_FRESNEL_TYPE_SPECULAR	= 3;
const uint BXDF_FRESNEL_TYPE_DISNEY		= 4;

struct Fresnel
{
	uint type;

	// Dieletric -> Only use data0.x and data1.x for the coefficients of refraction
	// Conductor -> data0 as etaI, data1 as etaT, data2 as k
	// Specular -> data0 as R, data1 as T, data2.x as etaA, data2.y as etaB
	// Disney -> data0->R0, data1.x->eta, data1.y->metallic
	vec3 data0;
	vec3 data1;
	vec3 data2;
};

vec3 BXDF_Fresnel_evaluate(Fresnel fresnel, float cosThetaI)
{
	switch(fresnel.type)
	{
		case BXDF_FRESNEL_TYPE_DIELETRIC:
			{
				float etaI = fresnel.data0.x;
				float etaT = fresnel.data1.x;
				float Fr = FrDieletric(cosThetaI, etaI, etaT);
				return vec3(Fr, Fr, Fr);
			}
		case BXDF_FRESNEL_TYPE_CONDUCTOR:
			{
				vec3 etaI = fresnel.data0;
				vec3 etaT = fresnel.data1;
				vec3 k = fresnel.data2;
				return FrConductor(cosThetaI, etaI, etaT, k);
			}
		case BXDF_FRESNEL_TYPE_DISNEY:
			{
				vec3 R0 = fresnel.data0;
				float eta = fresnel.data1.x;
				float metallic = fresnel.data1.y;
				float d = FrDieletric(cosThetaI, 1, eta);
				return mix(vec3(d,d,d), FrSchlick(R0, cosThetaI), metallic);
			}
		case BXDF_FRESNEL_TYPE_SPECULAR:
		case BXDF_FRESNEL_TYPE_NOOP:
		default:
			return vec3(1,1,1);
	}
}

#endif// BXDF_FRESNEL_GLSL
