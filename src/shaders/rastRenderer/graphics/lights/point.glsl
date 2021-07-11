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
	float radiusMax = 100.0f;// 100 meters
	float radiusMin = 0.001f;// 1 millimeter
	float distLight = length(dist);

	// Clamp minimum distance
	float r = max(distLight, radiusMin);

	// Calculate falloff (clamp maximum)
	float d = distLight/radiusMax;
	float d2 = d*d;
	float nom = max(1-(d2*d2),0);
	float falloff = nom*nom;

	// Calculate light intensity
	vec3 I = light.datav[0].xyz;
	return falloff*I/(r*r);
}

#endif// ATTA_RAST_LIGHTS_POINT_GLSL
