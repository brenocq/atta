//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// distant.glsl
// Date: 2021-03-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_LIGHTS_DISTANT_GLSL
#define ATTA_RAST_LIGHTS_DISTANT_GLSL

vec3 DistantLight_sampleLi(Light light, out vec3 wi)
{
	vec3 I = light.datav[0].xyz;
	vec3 wLight = light.datav[1].xyz;
	wi = wLight;

	return I;
}

#endif// ATTA_RAST_LIGHTS_DISTANT_GLSL
