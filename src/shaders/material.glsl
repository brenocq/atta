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
const uint MATERIAL_TYPE_DISNEY	 		= 4;
const uint MATERIAL_TYPE_UNREAL_ENGINE_4= 5;
const uint MATERIAL_TYPE_UBER			= 6;
const uint MATERIAL_TYPE_GLASS			= 7;
const uint MATERIAL_TYPE_SUBSTRATE		= 8;

struct Material 
{
	// Support to up to 8 materials
	uint type[1];

	// General data about the material
	int datai[16];
	float dataf[16];
	vec4 datav[8];
};

//---------- Data description ----------//
//----- DIFFUSE MATERIAL -----
// - datai
// 		[0](vec3 texture) kd texture index
// 		[1](float texture) sigma texture index
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
// 		[1](float texture) metallic texture index
// 		[2](float texture) roughness texture index
// 		[3](float texture) subsurface texture index
// 		[4](float texture) specular texture index
// 		[5](float texture) specular tint texture index
// 		[6](float texture) eta texture index
// 		[7](float texture) anisotropic texture index
// 		[8](float texture) sheen texture index
// 		[9](float texture) sheen tint texture index
// 		[10](float texture) clear coat texture index
// 		[11](float texture) clear coat gloss texture index
// 		[12](float texture) transmission texture index
// 		[13](float texture) bumpMap texture index
// - dataf
// 		[0] uniform metallic
// 		[1] uniform roughness
// 		[2] uniform subsurface
// 		[3] uniform specular
// 		[4] uniform specular tint
// 		[5] uniform eta
// 		[6] uniform anisotropic
// 		[7] uniform sheen
// 		[8] uniform sheen tint
// 		[9] uniform clear coat
// 		[10] uniform clear coat gloss
// 		[11] uniform transmission
// 		[12] uniform bumpMap
// - datav
// 		[0] uniform color
//
//----- UNREAL ENGINE 4 MATERIAL -----
// - datai
// 		[0](vec3 texture) albedo texture index
// 		[1](float texture) metallic texture index
// 		[2](float texture) roughness texture index
// 		[3](float texture) ao texture index
// - dataf
// 		[0] uniform metallic
// 		[1] uniform roughness
// 		[2] uniform ao
// - datav
// 		[0] uniform albedo
//
//----- UBER MATERIAL -----
// - datai
// 		[0](vec3 texture) Kd texture index
// 		[1](vec3 texture) Ks texture index
// 		[2](vec3 texture) Kr texture index
// 		[3](vec3 texture) Kt texture index
// 		[4](float texture) roughness texture index
// 		[5](float texture) roughnessu texture index
// 		[6](float texture) roughnessv texture index
// 		[7](vec3 texture) opacity texture index
// 		[8](float texture) eta texture index
// 		[9](float texture) bumpMap texture index
// 		[10](bool) active->remap urough and vrough from [0,1] to expected alpha
// - dataf
// 		[0] uniform roughness
// 		[1] uniform roughnessu
// 		[2] uniform roughnessv
// 		[3] uniform eta
// 		[4] uniform bumpMap
// - datav
// 		[0] uniform Kd
// 		[1] uniform Ks
// 		[2] uniform Kr
// 		[3] uniform Kt
// 		[4] uniform opacity
//----- GLASS MATERIAL -----
// - datai
// 		[0](vec3 texture) Kr texture index
// 		[1](vec3 texture) Kt texture index
// 		[2](float texture) roughness texture index
// 		[3](float texture) roughnessu texture index
// 		[4](float texture) roughnessv texture index
// 		[5](float texture) eta texture index
// 		[6](float texture) bumpMap texture index
// 		[7](bool) active->remap urough and vrough from [0,1] to expected alpha
// - dataf
// 		[0] uniform roughness
// 		[1] uniform roughnessu
// 		[2] uniform roughnessv
// 		[3] uniform eta
// 		[4] uniform bumpMap
// - datav
// 		[0] uniform Kr
// 		[1] uniform Kt
//----- SUBSTRATE MATERIAL -----
// - datai
// 		[0](vec3 texture) Kd texture index
// 		[1](vec3 texture) Ks texture index
// 		[2](float texture) roughness texture index
// 		[3](float texture) roughnessu texture index
// 		[4](float texture) roughnessv texture index
// 		[5](float texture) bumpMap texture index
// 		[6](bool) active->remap urough and vrough from [0,1] to expected alpha
// - dataf
// 		[0] uniform roughness
// 		[1] uniform roughnessu
// 		[2] uniform roughnessv
// 		[4] uniform bumpMap
// - datav
// 		[0] uniform Kd
// 		[1] uniform Ks

#endif// MATERIAL_GLSL
