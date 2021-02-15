//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// base.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_BASE_GLSL
#define LIGHTS_BASE_GLSL
#include "../bxdf/base.glsl"

const uint LIGHT_TYPE_DIFFUSE 		= 0;
const uint LIGHT_TYPE_DISTANT 		= 1;
const uint LIGHT_TYPE_GONIOMETRIC 	= 2;
const uint LIGHT_TYPE_INFINITE 		= 3;
const uint LIGHT_TYPE_POINT	 		= 4;
const uint LIGHT_TYPE_PROJECTION 	= 5;
const uint LIGHT_TYPE_SPOT 			= 6;

const uint LIGHT_FLAG_DELTA_POSITION 	= 0x00000001u;
const uint LIGHT_FLAG_DELTA_DIRECTION 	= 0x00000002u;
const uint LIGHT_FLAG_AREA 				= 0x00000004u;
const uint LIGHT_FLAG_INFINITE 			= 0x00000008u;

struct Light 
{
	uint type;

	uint nSamples;

	mat4 lightToWorld;// It must not be scaled!
	mat4 worldToLight;
};

struct Interaction
{
	vec3 point;// Point of interaction
	vec3 wo;
	vec3 n;// Surface normal

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

#endif// LIGHTS_BASE_GLSL
