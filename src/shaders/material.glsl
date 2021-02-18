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
//
//----- MIRROR MATERIAL -----
// - datav
// 		[0](vec3) - R
//
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
//
//----- DISNEY MATERIAL -----
// - datai
// 		[0](vec3 texture) color texture index
// 		[1](vec3 texture) metallic texture index
// 		[2](float texture) eta texture index
// 		[3](float texture) roughness texture index
// 		[4](float texture) specularTint texture index
// 		[5](float texture) anisotropic texture index
// 		[6](float texture) sheen texture index
// 		[7](float texture) sheenTint texture index
// 		[8](float texture) clearCoat texture index
// 		[9](float texture) clearCoatGloss texture index
// 		[10](float texture) specTrans texture index
// 		[11](vec3 texture) scatterDistance texture index
// 		[12](bool) thin
// 		[13](float texture) flatness texture index
// 		[14](float texture) diffTrans texture index
// 		[15](float texture) bumpMap texture index
// - dataf
// 		[0] uniform eta
// 		[1] uniform roughness
// 		[2] uniform specularTint
// 		[3] uniform anisotropic
// 		[4] uniform sheen
// 		[5] uniform sheenTint
// 		[6] uniform clearCoat
// 		[7] uniform clearCoatGloss
// 		[8] uniform specTrans
// 		[9] uniform flatness
// 		[10] uniform diffTrans
// 		[11] uniform bumpMap
// - datav
// 		[0] uniform color
// 		[1] uniform metallic
// 		[2] uniform scatterDistance

#endif// MATERIAL_GLSL
