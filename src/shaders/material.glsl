//--------------------------------------------------
// Atta GLSL
// material.glsl
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIAL_GLSL
#define MATERIAL_GLSL

const uint MATERIAL_TYPE_NONE		 	= 0;
const uint MATERIAL_TYPE_DIFFUSE	 	= 1;
const uint MATERIAL_TYPE_MIRROR	 		= 2;

struct Material 
{
	// Support to up to 8 materials
	uint type[8];

	// General data about the material
	uint datai[16];
	float dataf[16];
	vec4 datav[16];
};

//---------- Data description ----------//
// Diffuse Material
// - dataf
// 		[0] - sigma
// - datav
// 		[0](vec3) - Kd

#endif// MATERIAL_GLSL
