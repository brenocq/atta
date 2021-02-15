//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// camera.glsl
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CAMERA_GLSL
#define CAMERA_GLSL
#include "samplers/random.glsl"

struct CameraRay
{
	vec3 origin;
	vec3 direction;
};

CameraRay generateRayProjectiveCamera(
		// Basic
		mat4 projInv, mat4 viewInv, vec2 xy, 
		// Thin Lens Model variables (lensRadius=0 to use ideal pinhole camera)
		float lensRadius, float focusDistance, vec2 sampleLens)
{
	CameraRay ray;
	// Calculate origin and direction (camera space)
	vec3 origin = vec3(0,0,0);
	vec3 direction = (projInv*vec4(xy,1,1)).xyz;

	if(lensRadius > 0)	
	{
		// Sample point in lens
		vec2 pLens = lensRadius*concentricSampleDisk(sampleLens);

		// Compute point on plane of focus
		vec3 pFocus = focusDistance*direction;

		// Update ray for effect of lens
		origin = vec3(pLens, 0);
		direction = pFocus-origin;
	}

	// Camera space to world space
	ray.origin = (viewInv*vec4(origin,1)).xyz;
	ray.direction = (viewInv*vec4(normalize(direction), 0)).xyz;

	return ray;
}

CameraRay generateRayOrthographicCamera(mat4 viewMat, vec2 xy)
{
	CameraRay ray;
	ray.origin = vec3(1,1,0);
	ray.direction =  vec3(0, -1, 0);

	return ray;
}

#endif// CAMERA_GLSL
