//--------------------------------------------------
// Atta Graphics
// material.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_CORE_MATERIAL_H
#define ATTA_GRAPHICS_CORE_MATERIAL_H

#include <atta/math/math.h>
#include <atta/helpers/log.h>

namespace atta
{
	struct Material
	{
		alignas(4)	unsigned type[1] = {MATERIAL_TYPE_NONE};
		alignas(4)	int datai[16];
		alignas(4)	float dataf[16];
		alignas(16)	vec4 datav[8];

		enum Type
		{
			MATERIAL_TYPE_NONE = 0,
			MATERIAL_TYPE_DIFFUSE,
			MATERIAL_TYPE_MIRROR,
			MATERIAL_TYPE_METAL,
			MATERIAL_TYPE_DISNEY,
			MATERIAL_TYPE_UNREAL_ENGINE_4,
			MATERIAL_TYPE_UBER,
			MATERIAL_TYPE_GLASS,
			MATERIAL_TYPE_SUBSTRATE,
		};

		//------------------- Diffuse ---------------------//
		struct DiffuseInfo {
			int kdTexture = -1;
			int sigmaTexture = -1;
			vec3 kd = {1,0,1};
			float sigma = 20;
		};
		static Material diffuse(DiffuseInfo info)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_DIFFUSE;
			m.datai[0] = info.kdTexture;
			m.datai[1] = info.sigmaTexture;
			m.datav[0] = vec4(info.kd,-1);
			m.dataf[0] = info.sigma;

			return m;
		}

		//------------------- Mirror ---------------------//
		static Material mirror(vec3 R=vec3(1,1,1))
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_MIRROR;
			m.datav[0] = vec4(R,-1);

			return m;
		}

		//------------------- Metal ---------------------//
		struct MetalInfo {
			int etaTexture = -1;
			int kTexture = -1;
			int roughnessTexture = -1;
			int uroughnessTexture = -1;
			int vroughnessTexture = -1;
			int bumpTexture = -1;
			bool remapRoughness = true;

			vec3 eta = {1,1,1};
			vec3 k = {1,1,1};
			float roughness = 0.01f;
			float uroughness = -1;
			float vroughness = -1;
			float bump = 0;
		};
		static Material metal(MetalInfo info)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_METAL;

			m.datai[0] = info.etaTexture;
			m.datai[1] = info.kTexture;
			m.datai[2] = info.roughnessTexture;
			m.datai[3] = info.uroughnessTexture;
			m.datai[4] = info.vroughnessTexture;
			m.datai[5] = info.bumpTexture;
			m.datai[6] = info.remapRoughness?1:0;

			m.dataf[0] = info.roughness;
			m.dataf[1] = info.uroughness;
			m.dataf[2] = info.vroughness;
			m.dataf[3] = info.bump;

			m.datav[0] = vec4(info.eta,-1);
			m.datav[1] = vec4(info.k,-1);

			return m;
		}

		//------------------- Uber ---------------------//
		struct UberInfo {
			int kdTexture = -1;
			int ksTexture = -1;
			int krTexture = -1;
			int ktTexture = -1;
			int roughnessTexture = -1;
			int uroughnessTexture = -1;
			int vroughnessTexture = -1;
			int opacityTexture = -1;
			int etaTexture = -1;
			int bumpTexture = -1;
			bool remapRoughness = true;

			vec3 kd = {.25f,.25f,.25f};
			vec3 ks = {.25f,.25f,.25f};
			vec3 kr = {0,0,0};
			vec3 kt = {0,0,0};
			vec3 opacity = {1,1,1};
			float roughness = 1.0f;
			float uroughness = -1;
			float vroughness = -1;
			float eta = 1.5f;
			float bump = 0;
		};
		static Material uber(UberInfo info)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_UBER;

			m.datai[0] = info.kdTexture;
			m.datai[1] = info.ksTexture;
			m.datai[2] = info.krTexture;
			m.datai[3] = info.ktTexture;
			m.datai[4] = info.roughnessTexture;
			m.datai[5] = info.uroughnessTexture;
			m.datai[6] = info.vroughnessTexture;
			m.datai[7] = info.opacityTexture;
			m.datai[8] = info.etaTexture;
			m.datai[9] = info.bumpTexture;
			m.datai[10] = info.remapRoughness?1:0;

			m.dataf[0] = info.roughness;
			m.dataf[1] = info.uroughness;
			m.dataf[2] = info.vroughness;
			m.dataf[3] = info.eta;
			m.dataf[4] = info.bump;

			m.datav[0] = vec4(info.kd,-1);
			m.datav[1] = vec4(info.ks,-1);
			m.datav[2] = vec4(info.kr,-1);
			m.datav[3] = vec4(info.kt,-1);
			m.datav[4] = vec4(info.opacity,-1);

			return m;
		}

		//------------------- Glass ---------------------//
		struct GlassInfo {
			int krTexture = -1;
			int ktTexture = -1;
			int roughnessTexture = -1;
			int uroughnessTexture = -1;
			int vroughnessTexture = -1;
			int etaTexture = -1;
			int bumpTexture = -1;
			bool remapRoughness = true;

			vec3 kr = {1,1,1};
			vec3 kt = {1,1,1};
			float roughness = 0.0f;
			float uroughness = -1;
			float vroughness = -1;
			float eta = 1.5f;
			float bump = 0;
		};
		static Material glass(GlassInfo info)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_GLASS;

			m.datai[0] = info.krTexture;
			m.datai[1] = info.ktTexture;
			m.datai[2] = info.roughnessTexture;
			m.datai[3] = info.uroughnessTexture;
			m.datai[4] = info.vroughnessTexture;
			m.datai[5] = info.etaTexture;
			m.datai[6] = info.bumpTexture;
			m.datai[7] = info.remapRoughness?1:0;

			m.dataf[0] = info.roughness;
			m.dataf[1] = info.uroughness;
			m.dataf[2] = info.vroughness;
			m.dataf[3] = info.eta;
			m.dataf[4] = info.bump;

			m.datav[0] = vec4(info.kr,-1);
			m.datav[1] = vec4(info.kt,-1);

			return m;
		}

		//------------------- Substrate ---------------------//
		struct SubstrateInfo {
			int kdTexture = -1;
			int ksTexture = -1;
			int roughnessTexture = -1;
			int uroughnessTexture = -1;
			int vroughnessTexture = -1;
			int bumpTexture = -1;
			bool remapRoughness = true;

			vec3 kd = {.5,.5,.5};
			vec3 ks = {.5,.5,.5};
			float roughness = 0.1f;
			float uroughness = -1;
			float vroughness = -1;
			float bump = 0;
		};
		static Material substrate(SubstrateInfo info)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_SUBSTRATE;

			m.datai[0] = info.kdTexture;
			m.datai[1] = info.ksTexture;
			m.datai[2] = info.roughnessTexture;
			m.datai[3] = info.uroughnessTexture;
			m.datai[4] = info.vroughnessTexture;
			m.datai[5] = info.bumpTexture;
			m.datai[6] = info.remapRoughness?1:0;

			m.dataf[0] = info.roughness;
			m.dataf[1] = info.uroughness;
			m.dataf[2] = info.vroughness;
			m.dataf[3] = info.bump;

			m.datav[0] = vec4(info.kd,-1);
			m.datav[1] = vec4(info.ks,-1);

			return m;
		}

		//------------------- Disney ---------------------//
		struct DisneyInfo {
			int colorTexture = -1;
			int metallicTexture = -1;
			int etaTexture = -1;
			int roughnessTexture = -1;
			int specularTintTexture = -1;
			int anisotropicTexture = -1;
			int sheenTexture = -1;
			int sheenTintTexture = -1;
			int clearCoatTexture = -1;
			int clearCoatGlossTexture = -1;
			int specularTransTexture = -1;
			int scatterDistanceTexture = -1;
			int flatnessTexture = -1;
			int diffTransTexture = -1;
			int bumpMapTexture = -1;

			vec3 color;
			float metallic;
			float eta;
			float roughness;
			float specularTint;
			float anisotropic;
			float sheen;
			float sheenTint;
			float clearCoat;
			float clearCoatGloss;
			float specularTrans;
			vec3 scatterDistance;
			float flatness;
			float diffTrans;
			float bumpMap;
			bool thin;
		};
		static Material disney(DisneyInfo info)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_DISNEY;

			m.datai[0]  = info.colorTexture;
			m.datai[1]  = info.metallicTexture;
			m.datai[2]  = info.etaTexture;
			m.datai[3]  = info.roughnessTexture;
			m.datai[4]  = info.specularTintTexture;
			m.datai[5]  = info.anisotropicTexture;
			m.datai[6]  = info.sheenTexture;
			m.datai[7]  = info.sheenTintTexture;
			m.datai[8]  = info.clearCoatTexture;
			m.datai[9]  = info.clearCoatGlossTexture;
			m.datai[10] = info.specularTransTexture;
			m.datai[11] = info.scatterDistanceTexture;
			m.datai[12] = info.thin?1:0;
			m.datai[13] = info.flatnessTexture;
			m.datai[14] = info.diffTransTexture;
			m.datai[15] = info.bumpMapTexture;

			m.dataf[0]  = info.metallic;
			m.dataf[1]  = info.eta;
			m.dataf[2]  = info.roughness;
			m.dataf[3]  = info.specularTint;
			m.dataf[4]  = info.anisotropic;
			m.dataf[5]  = info.sheen;
			m.dataf[6]  = info.sheenTint;
			m.dataf[7]  = info.clearCoat;
			m.dataf[8]  = info.clearCoatGloss;
			m.dataf[9]  = info.specularTrans;
			m.dataf[10]  = info.flatness;
			m.dataf[11] = info.diffTrans;
			m.dataf[12] = info.bumpMap;

			m.datav[0] = vec4(info.color, -1);
			m.datav[1] = vec4(info.scatterDistance, -1);

			return m;
		}

		//------------------- Disney ---------------------//
		struct UnrealEngine4Info {
			int albedoTexture = -1;
			int metallicTexture = -1;
			int roughnessTexture = -1;
			int aoTexture = -1;

			vec3 albedo;
			float metallic;
			float roughness;
			float ao;
		};
		static Material unrealEngine4(UnrealEngine4Info info)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_UNREAL_ENGINE_4;

			m.datai[0]  = info.albedoTexture;
			m.datai[1]  = info.metallicTexture;
			m.datai[2]  = info.roughnessTexture;
			m.datai[3]  = info.aoTexture;

			m.dataf[0]  = info.metallic;
			m.dataf[1]  = info.roughness;
			m.dataf[2]  = info.ao;

			m.datav[0] = vec4(info.albedo, -1);

			return m;
		}

		std::string toString()
		{
			switch(type[0])
			{
				case MATERIAL_TYPE_DIFFUSE:
					return std::string("Material{type=DIFFUSE")+
						", KdTexture="+std::to_string(datai[0])+
						", sigmaTexture="+std::to_string(datai[1])+
						", Kd="+datav[0].toString()+
						", sigma="+std::to_string(dataf[0])+"}";
				case MATERIAL_TYPE_MIRROR:
					return std::string("Material{type=MIRROR")+
						", R="+datav[0].toString()+"}";
				default:
					return std::string("Material{type=")+std::to_string(type[0])+"}";
			}
		}
	};
}

#endif// ATTA_GRAPHICS_CORE_MATERIAL_H
