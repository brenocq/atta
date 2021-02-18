//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disney.glsl
// Date: 2021-02-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_DISNEY_GLSL
#define MATERIALS_DISNEY_GLSL
#include "../base.glsl"
#include "../../base.glsl"
#include "../../bxdf/base.glsl"
#include "../../bxdf/fresnel.glsl"
#include "../../bxdf/microfacetDistribution.glsl"

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

BXDF Material_Disney_computeScatteringFunctions(inout Material material, vec2 uv)
{
	BXDF bxdf;

	return bxdf;
}

#endif// MATERIALS_DISNEY_GLSL
