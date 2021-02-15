//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// base.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_BASE_GLSL
#define BXDF_BASE_GLSL
#include "../samplers/random.glsl"

const uint BXDF_TYPE_SPECULAR_REFLECTION 		= 0;
const uint BXDF_TYPE_SPECULAR_TRANSMISSION 		= 1;
const uint BXDF_TYPE_LAMBERTIAN_REFLECTION	 	= 2;
const uint BXDF_TYPE_LAMBERTIAN_TRANSMISSION	= 3;
const uint BXDF_TYPE_OREN_NAYAR					= 4;

const uint BXDF_FLAG_NONE		 	= 0x00000000u;
const uint BXDF_FLAG_REFLECTION 	= 0x00000001u;
const uint BXDF_FLAG_TRANSMISSION 	= 0x00000002u;
const uint BXDF_FLAG_DIFFUSE		= 0x00000004u;
const uint BXDF_FLAG_GLOSSY			= 0x00000008u;
const uint BXDF_FLAG_SPECULAR		= 0x00000010u;
const uint BXDF_FLAG_ALL			= BXDF_FLAG_REFLECTION | BXDF_FLAG_TRANSMISSION | BXDF_FLAG_DIFFUSE | BXDF_FLAG_GLOSSY | BXDF_FLAG_SPECULAR;

struct BXDF
{
	uint type;
	// TODO implement bxdf data
};

struct BSDF
{
	BXDF bxdf;
	float eta;
	vec3 ng;// Normal geometry (from mesh)
	vec3 ns;// Normal shading (TODO implement from bump map)
	vec3 ss;// primary tangent shading
	vec3 ts;// secondart tangent shading
};

float cosTheta(vec3 w) { return w.z; }
float cos2Theta(vec3 w) { return w.z*w.z; }
float absCosTheta(vec3 w) { return abs(w.z); }
float sin2Theta(vec3 w) { return max(0, 1-cos2Theta(w)); }
float sinTheta(vec3 w) { return sqrt(sin2Theta(w)); }
float tanTheta(vec3 w) { return sinTheta(w)/cosTheta(w); }
float tan2Theta(vec3 w) { return sin2Theta(w)/cos2Theta(w); }
float cosPhi(vec3 w)
{ 
	float sinT = sinTheta(w);
	return (sinT==0) ? 1 : clamp(w.x/sinT, -1, 1);
}
float sinPhi(vec3 w)
{ 
	float sinT = sinTheta(w);
	return (sinT==0) ? 0 : clamp(w.y/sinT, -1, 1);
}
float cos2Phi(vec3 w) { return cosPhi(w)*cosPhi(w); }
float sin2Phi(vec3 w) { return sinPhi(w)*sinPhi(w); }
float cosDPhi(vec3 wa, vec3 wb)
{
	// Project vectors to plane, normalize them, and take dot product to get the cossine
	return clamp((wa.x * wb.x + wa.y * wb.y) /
                 sqrt((wa.x * wa.x + wa.y * wa.y) *
                    (wb.x * wb.x + wb.y * wb.y)), -1, 1);
}

// Fresnel reflection for dieletric materials
float FrDieletric(float cosThetaI, float etaI, float etaT)
{
	// [in] etaI - index of refraction of incident medium
	// [in] etaT - index of refraction of transmitted medium
	
	cosThetaI = clamp(cosThetaI, -1, 1);
	// Potentially swap indices of refraction
	bool entering = cosThetaI > 0.f;
	if(!entering)
	{       
		float temp = etaT;
		etaT = etaI;
		etaI = temp;
		cosThetaI = abs(cosThetaI);
	}

	// Compute cosThetaT using Snell's law
	float sinThetaI = sqrt(max(0.f, 1.f - cosThetaI * cosThetaI));
	float sinThetaT = etaI / etaT * sinThetaI;

	// Handle total internal reflection
	if(sinThetaT >= 1) return 1;   
	float cosThetaT = sqrt(max(0.f, 1.f - sinThetaT * sinThetaT));
	float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
		   		  ((etaT * cosThetaI) + (etaI * cosThetaT));
	float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
				  ((etaI * cosThetaI) + (etaT * cosThetaT));
	return (Rparl * Rparl + Rperp * Rperp) * 0.5f;
}

vec3 FrConductor(float cosThetaI, vec3 etai, vec3 etat, vec3 k)
{
    cosThetaI = clamp(cosThetaI, -1, 1);
    vec3 eta = etat / etai;
    vec3 etak = k / etai;
  
    float cosThetaI2 = cosThetaI * cosThetaI;
    float sinThetaI2 = 1. - cosThetaI2;
    vec3 eta2 = eta * eta;
    vec3 etak2 = etak * etak;   
  
    vec3 t0 = eta2 - etak2 - sinThetaI2; 
    vec3 a2plusb2 = sqrt(t0 * t0 + 4 * eta2 * etak2);
    vec3 t1 = a2plusb2 + cosThetaI2;
    vec3 a = sqrt(0.5f * (a2plusb2 + t0)); 
    vec3 t2 = 2.0f * cosThetaI * a;
    vec3 Rs = (t1 - t2) / (t1 + t2);
  
    vec3 t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
    vec3 t4 = t2 * sinThetaI2;  
    vec3 Rp = Rs * (t3 - t4) / (t3 + t4);

    return 0.5 * (Rp + Rs);
}

bool refractWi(vec3 wi, vec3 n, float eta, out vec3 wt)
{
	float cosThetaI	= dot(n, wi);
	float sin2ThetaI = max(0.f, 1.f-cosThetaI*cosThetaI);
	float sin2ThetaT = eta*eta*sin2ThetaI;
	if(sin2ThetaT >= 1) return false;

	float cosThetaT = sqrt(1-sin2ThetaT);
	wt = eta*-wi + (eta*cosThetaI - cosThetaT)*n;
	return true;
}

vec3 cosineSampleHemisphere(vec2 u)
{
    vec2 d = concentricSampleDisk(u);
    float z = sqrt(max(0.f, 1.f - d.x * d.x - d.y * d.y));
    return vec3(d.x, d.y, z);
}

bool sameHemisphere(vec3 w, vec3 wp)
{
    return w.z * wp.z > 0;
}

#endif// BXDF_BASE_GLSL
