//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// point.glsl
// Date: 2021-03-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_LIGHTS_POINT_GLSL
#define ATTA_RAST_LIGHTS_POINT_GLSL

vec3 PointLight_sampleLi(Light light, out vec3 wi)
{
	vec3 pLight = (light.lightToWorld * vec4(0,0,0,1)).xyz;
	
	vec3 dist = pLight - inPos;
	wi = normalize(dist);

	vec3 I = light.datav[0].xyz;
	return I/dot(dist,dist);
}

#endif// ATTA_RAST_LIGHTS_POINT_GLSL
