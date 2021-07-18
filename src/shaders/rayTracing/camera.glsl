//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// camera.glsl
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CAMERA_GLSL
#define CAMERA_GLSL
#include "samplers/random.glsl"
 
const int CAMERA_THIN_LENS = 1;

void generateCameraRayThinLens(inout uint pixelSeed, out vec3 origin, out vec3 direction)
{
	// Thin Lens Model variables (lensRadius=0 to use ideal pinhole camera)
	float lensRadius = 0.00125;
	float focusDistance = 17.32/2;

	// Sample pixel (TODO use specific 2D sampler algorithm to improve results)
	const vec2 pixel = vec2(gl_LaunchIDEXT.x+randomFloat(pixelSeed)-0.5, gl_LaunchIDEXT.y+randomFloat(pixelSeed)-0.5);
	const vec2 xy = (pixel / gl_LaunchSizeEXT.xy) * 2.0 - 1.0;
	const vec2 sampleLens = vec2(randomFloat(pixelSeed), randomFloat(pixelSeed));

	// Calculate origin and direction (camera space)
	origin = vec3(0,0,0);
	direction = (ubo.projMatInverse*vec4(xy,1,1)).xyz;

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
	origin = (ubo.viewMatInverse*vec4(origin,1)).xyz;
	direction = (ubo.viewMatInverse*vec4(normalize(direction), 0)).xyz;
}

void generateCameraRay(inout uint pixelSeed, out vec3 origin, out vec3 direction)
{
	if(CAMERA_THIN_LENS == 1)
	{
		generateCameraRayThinLens(pixelSeed, origin, direction);
	}
}

#endif// CAMERA_GLSL
