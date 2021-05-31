//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// specularTransmission.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_SPECULAR_TRANSMISSION_GLSL
#define BXDF_SPECULAR_TRANSMISSION_GLSL
#include "base.glsl"
#include "fresnel.glsl"

vec3 BXDF_SpecularTransmission_f()
{
	return vec3(0,0,0);
}

vec3 BXDF_SpecularTransmission_sampleF(
		vec3 wo, out vec3 wi, vec2 u, out float pdf,
		Fresnel fresnel, vec3 T)
{
	// Figure out which n is incident and which is transmitted
	bool entering = cosTheta(wo)>0;
	float etaI = fresnel.data0.x;
	float etaT = fresnel.data1.x;
	etaI = entering ? etaI : etaT;
	etaT = entering ? etaT : etaI;

	// Normal face forward
	vec3 n = (wo.z < 0.f) ? vec3(0,0,-1) : vec3(0,0,1);

	// Compute ray direction for specular transmission
	if(!refractWi(wo, n, etaI/etaT, wi)) 
		return vec3(0,0,0);

	pdf = 1;
	vec3 ft = T*(vec3(1,1,1) - BXDF_Fresnel_evaluate(fresnel, cosTheta(wi)));


	// TODO implement mode to scale ft when tracing way from the light
	// Rays from the camera do not need to be scaled
	// if(mode == bxdf.transportMode == TRANSPORT_MODE_RADIANCE)
	// 	ft *= (etaI*etaI)/(etaT*etaT);

	return ft / absCosTheta(wi);
}

uint BXDF_SpecularTransmission_flags()
{
	return BXDF_FLAG_TRANSMISSION | BXDF_FLAG_SPECULAR;
}

#endif// BXDF_SPECULAR_TRANSMISSION_GLSL
