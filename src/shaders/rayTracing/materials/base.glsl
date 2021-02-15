//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// base.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_BASE_GLSL
#define MATERIALS_BASE_GLSL

const uint MATERIAL_TYPE_MATTE	 		= 0;
const uint MATERIAL_TYPE_MIRROR	 		= 1;

struct Material 
{
	uint type;
};

#endif// MATERIALS_BASE_GLSL
