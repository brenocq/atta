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
const uint MATERIAL_TYPE_METAL	 		= 3;

struct Material 
{
	// Support to up to 8 materials
	uint type[8];

	// General data about the material
	int datai[16];
	float dataf[16];
	vec4 datav[16];
};

//---------- Data description ----------//
//----- DIFFUSE MATERIAL -----
// - dataf
// 		[0] - sigma
// - datav
// 		[0](vec3) - Kd
//----- MIRROR MATERIAL -----
// - datav
// 		[0](vec3) - R
//----- METAL MATERIAL -----
// - Obs:
// 		- Only rough should be used to isotropic roughess / urough and vrough for anisotropic roughness
// - datai
// 		[0](vec3 texture) eta texture index (IOR source/IOR destination)
// 		[1](vec3 texture) k texture index (absorption coefficient)
// 		[2](float texture) rough texture index
// 		[3](float texture) urough texture index (Trowbridge-Reitz alpha u)
// 		[4](float texture) vrough texture index (Trowbridge-Reitz alpha v)
// 		[5](float texture) bump texture index
// 		[6](bool) active->remap urough and vrough from [0,1] to expected alpha
// - dataf
// 		[0] uniform rough
// 		[1] uniform urough
// 		[2] uniform vrough
// 		[3] uniform bump
// - datav
// 		[0] uniform eta
// 		[1] uniform k

#endif// MATERIAL_GLSL
