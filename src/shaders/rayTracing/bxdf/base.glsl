//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// base.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_BASE_GLSL
#define BXDF_BASE_GLSL

const uint BSDF_REFLECTION 		= 0x00000001u;
const uint BSDF_TRANSMISSION 	= 0x00000002u;
const uint BSDF_DIFFUSE			= 0x00000004u;
const uint BSDF_GLOSSY			= 0x00000008u;
const uint BSDF_SPECULAR		= 0x00000010u;
const uint BSDF_ALL				= BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR;

float cosTheta(vec3 w) { return w.z; }
float cos2Theta(vec3 w) { return w.z*w.z; }
float absCosTheta(vec3 w) { return abs(w.z); }
float sin2Theta(vec3 w) { return max(0, 1-cos2theta(w)); }
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

#endif// BXDF_BASE_GLSL
