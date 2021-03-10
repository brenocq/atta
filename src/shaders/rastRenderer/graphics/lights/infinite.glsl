//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// infinite.glsl
// Date: 2021-03-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_LIGHTS_INFINITE_GLSL
#define ATTA_RAST_LIGHTS_INFINITE_GLSL
#include "base.glsl"

vec3 InfiniteLight_sampleLi(Light light, out vec3 wi)
{
	wi = normalize(inNormal);

	vec3 w = normalize((light.worldToLight*vec4(wi,0)).xyz);
	vec2 st = vec2(sphericalPhi(w)*inv2Pi, sphericalTheta(w)*invPi);
	return texture(textures[light.datai[2]], st).rgb;
}

#endif// ATTA_RAST_LIGHTS_INFINITE_GLSL
