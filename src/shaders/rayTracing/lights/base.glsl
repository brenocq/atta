//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// base.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_BASE_GLSL
#define LIGHTS_BASE_GLSL
#include "../bxdf/base.glsl"
#include "../../light.glsl"
#include "../base.glsl"

const uint LIGHT_FLAG_DELTA_POSITION 	= 0x00000001u;
const uint LIGHT_FLAG_DELTA_DIRECTION 	= 0x00000002u;
const uint LIGHT_FLAG_AREA 				= 0x00000004u;
const uint LIGHT_FLAG_INFINITE 			= 0x00000008u;

struct Interaction
{
	vec3 point;// Point of interaction
	vec3 wo;
	vec3 n;// Surface normal (TODO shading normal)

	// Surface iteraction
	BSDF bsdf;
};

struct VisibilityTester
{
	Interaction p0;
	Interaction p1;
};

bool isSurfaceInteraction(Interaction it)
{
	return !(it.n.x==0 && it.n.y==0 && it.n.z==0);
}

float sphericalTheta(vec3 v)
{
	return acos(clamp(v.z, -1, 1));
}

float sphericalPhi(vec3 v)
{
	float p = atan(v.y, v.x);
	return (p<0) ? (p+2*pi) : p;
}

float powerHeuristic(int nf, float fPdf, int ng, float gPdf)
{
	float f = nf*fPdf, g=ng*gPdf;
	return (f*f)/(f*f+g*g);
}

#endif// LIGHTS_BASE_GLSL
