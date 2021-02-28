//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// infinite.glsl
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_INFINITE_GLSL
#define LIGHTS_INFINITE_GLSL
#include "base.glsl"
#include "../base.glsl"

vec2 sampleContinuous(Light light, vec2 u, out float pdf)
{
	uint distribIndex = light.datai[1];
	vec3 distributionData = texture(textures[distribIndex], u).xyz;

	pdf = distributionData.z;
	return distributionData.xy;
}

vec3 InfiniteLight_sampleLi(
		Light light, VisibilityPoint vp, vec2 u, 
		out vec3 wi, out float pdf, out VisibilityTester vis)
{
	float mapPdf;

	vec2 uv = sampleContinuous(light, u, mapPdf);
	if(mapPdf == 0) return vec3(0,0,0);

	// Convert infinite light sample point to direction
	float theta = uv.y * pi, phi = uv.x * 2 * pi;
	float cosTheta = cos(theta), sinTheta = sin(theta);
	float sinPhi = sin(phi), cosPhi = cos(phi);
   	wi = (light.lightToWorld * vec4(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta, 0)).xyz;

	// Compute PDF for sampled infinite light direction (apply jacobian from plane to sphere)
	pdf = mapPdf/(2*pi*pi*sinTheta);
	if(sinTheta == 0) pdf = 0.0f;

	// Populate visibility tester and return texture value
	float worldRadius = light.dataf[0];
	vis.p0.point = vp.point;
	vis.p0.wo = vp.wo;
	vis.p0.n = vp.n;
	vis.p1.point = vp.point+wi*(2*worldRadius);

	//return texture(textures[light.datai[0]], uv).rgb;
	return texture(textures[light.datai[0]], uv).rgb;
}

vec3 InfiniteLight_Le(inout Light light, vec3 rayDirection)
{
	uint textureIndex = light.datai[0];
	vec3 w = normalize((light.worldToLight*vec4(rayDirection,0)).xyz);
	vec2 st = vec2(sphericalPhi(w)*inv2Pi, sphericalTheta(w)*invPi);
	return texture(textures[textureIndex], st).rgb;
	//return texture(textures[light.datai[1]], st).rgb;
}

float InfiniteLight_pdfLi(Light light, Interaction it, vec3 wi)
{
	// TODO distribution function
	return 0.1f;
}

vec3 InfiniteLight_power(inout Light light)
{
	vec3 precomputedPower = light.datav[1].xyz;
	return precomputedPower;
}

uint InfiniteLight_flags()
{
	return LIGHT_FLAG_INFINITE;
}

#endif// LIGHTS_POINT_GLSL
