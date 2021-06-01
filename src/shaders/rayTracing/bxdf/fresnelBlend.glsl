//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// fresnelBlend.glsl    
// Date: 2021-05-31
// By Breno Cunha Queiroz  
//--------------------------------------------------
#ifndef BXDF_FRESNEL_BLEND_GLSL
#define BXDF_FRESNEL_BLEND_GLSL
#include "../base.glsl"    
#include "base.glsl"       
#include "microfacetDistribution.glsl"       

float pow5(float f)
{
	return (f*f)*(f*f)*f;
}

vec3 BXDF_FresnelBlend_f(vec3 wo, vec3 wi, 
		vec3 Rd, vec3 Rs, MicrofacetDistribution distribution)
{
	vec3 diffuse = (28.0f/(23.0f*pi))*Rd*(vec3(1,1,1)-Rs)*
		(1-pow5(1-0.5f*absCosTheta(wi)))*
		(1-pow5(1-0.5f*absCosTheta(wo)));
	vec3 wh = wi+wo;
	if(wi.x==0 && wh.y==0 && wh.z==0) return vec3(0,0,0);
	wh = normalize(wh);

	vec3 schlickFresnel = Rs+pow5(1-dot(wi, wh))*(vec3(1,1,1)-Rs);

	float D = BXDF_MicrofacetDistribution_D(distribution, wh);
	vec3 specular = D/
		(4*abs(dot(wi, wh))*max(absCosTheta(wi), absCosTheta(wo)))*
		schlickFresnel;

    return diffuse+specular;
} 

float BXDF_FresnelBlend_pdf(vec3 wo, vec3 wi, 
		MicrofacetDistribution distribution)
{ 
	if(!sameHemisphere(wo, wi)) return 0;
	vec3 wh = normalize(wi+wo);
	float pdfWh = BXDF_MicrofacetDistribution_pdf(distribution, wo, wi);
    return 0.5f * (absCosTheta(wi)*invPi + pdfWh/(4*dot(wo, wh)));
}     
      
vec3 BXDF_FresnelBlend_sampleF(vec3 wo, out vec3 wi, vec2 uOrig, out float pdf,
		vec3 Rd, vec3 Rs, MicrofacetDistribution distribution)
{
	vec2 u = uOrig;
      
    if(u[0]<.5)
    { 
		u[0] = min(2*u[0], 0.99999f);
		wi = cosineSampleHemisphere(u);
		if(wo.z<0) wi.z*=-1;
    } 
    else
    {
		u[0] = min(2*(u[0]-0.5f), 0.99999f);
		vec3 wh = BXDF_MicrofacetDistribution_sampleWh(distribution, wo, u);
		wi = reflect(wo, wh);
		if(!sameHemisphere(wo, wi)) return vec3(0,0,0);
    }

	pdf = BXDF_FresnelBlend_pdf(wo, wi, distribution);
	return BXDF_FresnelBlend_f(wo, wi, Rd, Rs, distribution);
}

uint BXDF_FresnelBlend_flags()
{
    return BXDF_FLAG_REFLECTION | BXDF_FLAG_GLOSSY;
}

#endif// BXDF_FRESNEL_BLEND_GLSL
