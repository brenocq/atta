//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// matte.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_MATTE_GLSL
#define MATERIALS_MATTE_GLSL
#include "base.glsl"

void Material_Matte_computeScatteringFunctions(Material material, vec3 Kd, float sigma)
{
	if(Kd.x > 0 && Kd.y > 0 && Kd.z > 0)// If not black
	{
		if(sigma == 0)
		{
			// Add Lambertian
		}
		else
		{
			// Add Oren Nayar
		}
	}
}

#endif// MATERIALS_MATTE_GLSL
