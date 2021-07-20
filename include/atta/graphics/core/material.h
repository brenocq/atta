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
		alignas(4)	unsigned type = {MATERIAL_TYPE_NONE};
		alignas(4)	int datai[4];
		alignas(4)	float dataf[4];
		alignas(16)	vec4 datav[2];

		enum Type
		{
			MATERIAL_TYPE_NONE = 0,
			MATERIAL_TYPE_DIFFUSE,
			MATERIAL_TYPE_UBER,
			//MATERIAL_TYPE_MIRROR,
			//MATERIAL_TYPE_METAL,
			//MATERIAL_TYPE_DISNEY,
			//MATERIAL_TYPE_UNREAL_ENGINE_4,
			//MATERIAL_TYPE_GLASS,
			//MATERIAL_TYPE_SUBSTRATE,
		};

		//------------------- Diffuse ---------------------//
		struct DiffuseInfo {
			int kdTexture = -1;
			int sigmaTexture = -1;
			vec3 kd = {1,1,1};
			float sigma = 20;
		};
		static Material diffuse(DiffuseInfo info)
		{
			Material m;
			m.type = MATERIAL_TYPE_DIFFUSE;
			m.datai[0] = info.kdTexture;
			m.datai[1] = info.sigmaTexture;
			m.datav[0] = vec4(info.kd,-1);
			m.dataf[0] = info.sigma;

			return m;
		}

		//------------------- Uber ---------------------//
		struct UberInfo {
			int albedoTexture = -1;
			int metallicTexture = -1;
			int roughnessTexture = -1;
			int aoTexture = -1;

			vec3 albedo = vec3(1,1,1);
			float metallic = 0.0f;
			float roughness = 0.5f;
			float ao = 1.0;
		};
		static Material uber(UberInfo info)
		{
			Material m;
			m.type = MATERIAL_TYPE_UBER;

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

		//------------------- Mirror ---------------------//
		//static Material mirror(vec3 R=vec3(1,1,1))
		//{
		//	Material m;
		//	m.type[0] = MATERIAL_TYPE_MIRROR;
		//	m.datav[0] = vec4(R,-1);

		//	return m;
		//}

		//------------------- Metal ---------------------//
		//struct MetalInfo {
		//	int etaTexture = -1;
		//	int kTexture = -1;
		//	int roughnessTexture = -1;
		//	int uroughnessTexture = -1;
		//	int vroughnessTexture = -1;
		//	int bumpTexture = -1;
		//	bool remapRoughness = true;

		//	vec3 eta = {1,1,1};
		//	vec3 k = {1,1,1};
		//	float roughness = 0.01f;
		//	float uroughness = -1;
		//	float vroughness = -1;
		//	float bump = 0;
		//};
		//static Material metal(MetalInfo info)
		//{
		//	Material m;
		//	m.type[0] = MATERIAL_TYPE_METAL;

		//	m.datai[0] = info.etaTexture;
		//	m.datai[1] = info.kTexture;
		//	m.datai[2] = info.roughnessTexture;
		//	m.datai[3] = info.uroughnessTexture;
		//	m.datai[4] = info.vroughnessTexture;
		//	m.datai[5] = info.bumpTexture;
		//	m.datai[6] = info.remapRoughness?1:0;

		//	m.dataf[0] = info.roughness;
		//	m.dataf[1] = info.uroughness;
		//	m.dataf[2] = info.vroughness;
		//	m.dataf[3] = info.bump;

		//	m.datav[0] = vec4(info.eta,-1);
		//	m.datav[1] = vec4(info.k,-1);

		//	return m;
		//}

		////------------------- Uber ---------------------//
		//struct UberInfo {
		//	int kdTexture = -1;
		//	int ksTexture = -1;
		//	int krTexture = -1;
		//	int ktTexture = -1;
		//	int roughnessTexture = -1;
		//	int uroughnessTexture = -1;
		//	int vroughnessTexture = -1;
		//	int opacityTexture = -1;
		//	int etaTexture = -1;
		//	int bumpTexture = -1;
		//	bool remapRoughness = true;

		//	vec3 kd = {.25f,.25f,.25f};
		//	vec3 ks = {.25f,.25f,.25f};
		//	vec3 kr = {0,0,0};
		//	vec3 kt = {0,0,0};
		//	vec3 opacity = {1,1,1};
		//	float roughness = 1.0f;
		//	float uroughness = -1;
		//	float vroughness = -1;
		//	float eta = 1.5f;
		//	float bump = 0;
		//};
		//static Material uber(UberInfo info)
		//{
		//	Material m;
		//	m.type[0] = MATERIAL_TYPE_UBER;

		//	m.datai[0] = info.kdTexture;
		//	m.datai[1] = info.ksTexture;
		//	m.datai[2] = info.krTexture;
		//	m.datai[3] = info.ktTexture;
		//	m.datai[4] = info.roughnessTexture;
		//	m.datai[5] = info.uroughnessTexture;
		//	m.datai[6] = info.vroughnessTexture;
		//	m.datai[7] = info.opacityTexture;
		//	m.datai[8] = info.etaTexture;
		//	m.datai[9] = info.bumpTexture;
		//	m.datai[10] = info.remapRoughness?1:0;

		//	m.dataf[0] = info.roughness;
		//	m.dataf[1] = info.uroughness;
		//	m.dataf[2] = info.vroughness;
		//	m.dataf[3] = info.eta;
		//	m.dataf[4] = info.bump;

		//	m.datav[0] = vec4(info.kd,-1);
		//	m.datav[1] = vec4(info.ks,-1);
		//	m.datav[2] = vec4(info.kr,-1);
		//	m.datav[3] = vec4(info.kt,-1);
		//	m.datav[4] = vec4(info.opacity,-1);

		//	return m;
		//}

		////------------------- Glass ---------------------//
		//struct GlassInfo {
		//	int krTexture = -1;
		//	int ktTexture = -1;
		//	int roughnessTexture = -1;
		//	int uroughnessTexture = -1;
		//	int vroughnessTexture = -1;
		//	int etaTexture = -1;
		//	int bumpTexture = -1;
		//	bool remapRoughness = true;

		//	vec3 kr = {1,1,1};
		//	vec3 kt = {1,1,1};
		//	float roughness = 0.0f;
		//	float uroughness = -1;
		//	float vroughness = -1;
		//	float eta = 1.5f;
		//	float bump = 0;
		//};
		//static Material glass(GlassInfo info)
		//{
		//	Material m;
		//	m.type[0] = MATERIAL_TYPE_GLASS;

		//	m.datai[0] = info.krTexture;
		//	m.datai[1] = info.ktTexture;
		//	m.datai[2] = info.roughnessTexture;
		//	m.datai[3] = info.uroughnessTexture;
		//	m.datai[4] = info.vroughnessTexture;
		//	m.datai[5] = info.etaTexture;
		//	m.datai[6] = info.bumpTexture;
		//	m.datai[7] = info.remapRoughness?1:0;

		//	m.dataf[0] = info.roughness;
		//	m.dataf[1] = info.uroughness;
		//	m.dataf[2] = info.vroughness;
		//	m.dataf[3] = info.eta;
		//	m.dataf[4] = info.bump;

		//	m.datav[0] = vec4(info.kr,-1);
		//	m.datav[1] = vec4(info.kt,-1);

		//	return m;
		//}

		////------------------- Substrate ---------------------//
		//struct SubstrateInfo {
		//	int kdTexture = -1;
		//	int ksTexture = -1;
		//	int roughnessTexture = -1;
		//	int uroughnessTexture = -1;
		//	int vroughnessTexture = -1;
		//	int bumpTexture = -1;
		//	bool remapRoughness = true;

		//	vec3 kd = {.5,.5,.5};
		//	vec3 ks = {.5,.5,.5};
		//	float roughness = 0.1f;
		//	float uroughness = -1;
		//	float vroughness = -1;
		//	float bump = 0;
		//};
		//static Material substrate(SubstrateInfo info)
		//{
		//	Material m;
		//	m.type[0] = MATERIAL_TYPE_SUBSTRATE;

		//	m.datai[0] = info.kdTexture;
		//	m.datai[1] = info.ksTexture;
		//	m.datai[2] = info.roughnessTexture;
		//	m.datai[3] = info.uroughnessTexture;
		//	m.datai[4] = info.vroughnessTexture;
		//	m.datai[5] = info.bumpTexture;
		//	m.datai[6] = info.remapRoughness?1:0;

		//	m.dataf[0] = info.roughness;
		//	m.dataf[1] = info.uroughness;
		//	m.dataf[2] = info.vroughness;
		//	m.dataf[3] = info.bump;

		//	m.datav[0] = vec4(info.kd,-1);
		//	m.datav[1] = vec4(info.ks,-1);

		//	Return m;
		//}

		//------------------- Disney ---------------------//
		//struct DisneyInfo {
		//	int colorTexture = -1;
		//	int metallicTexture = -1;
		//	int roughnessTexture = -1;
		//	int subsurfaceTexture = -1;
		//	int specularTexture = -1;
		//	int specularTintTexture = -1;
		//	int etaTexture = -1;
		//	int anisotropicTexture = -1;
		//	int sheenTexture = -1;
		//	int sheenTintTexture = -1;
		//	int clearCoatTexture = -1;
		//	int clearCoatGlossTexture = -1;
		//	int transmissionTexture = -1;
		//	int bumpMapTexture = -1;

		//	vec3 color = vec3(1,1,1);
		//	float metallic = 0.0f;
		//	float roughness = 0.5f;
		//	float subsurface = 0.0f;
		//	float specular = 0.5f;
		//	float specularTint = 0.0f;
		//	float eta = 1.45;
		//	float anisotropic = 0.0f;
		//	float sheen = 0.0f;
		//	float sheenTint = 0.0f;
		//	float clearCoat = 0.0f;
		//	float clearCoatGloss = 1.0f;
		//	float transmission = 0.0f;
		//	float bumpMap = 0.0f;
		//};
		//static Material disney(DisneyInfo info)
		//{
		//	Material m;
		//	m.type[0] = MATERIAL_TYPE_DISNEY;

		//	m.datai[0]  = info.colorTexture;
		//	m.datai[1]  = info.metallicTexture;
		//	m.datai[2]  = info.roughnessTexture;
		//	m.datai[3]  = info.subsurfaceTexture;
		//	m.datai[4]  = info.specularTexture;
		//	m.datai[5]  = info.specularTintTexture;
		//	m.datai[6]  = info.etaTexture;
		//	m.datai[7]  = info.anisotropicTexture;
		//	m.datai[8]  = info.sheenTexture;
		//	m.datai[9]  = info.sheenTintTexture;
		//	m.datai[10] = info.clearCoatTexture;
		//	m.datai[11] = info.clearCoatGlossTexture;
		//	m.datai[12] = info.transmissionTexture;
		//	m.datai[13] = info.bumpMapTexture;

		//	m.dataf[0]  = info.metallic;
		//	m.dataf[1]  = info.roughness;
		//	m.dataf[2]  = info.subsurface;
		//	m.dataf[3]  = info.specular;
		//	m.dataf[4]  = info.specularTint;
		//	m.dataf[5]  = info.eta;
		//	m.dataf[6]  = info.anisotropic;
		//	m.dataf[7]  = info.sheen;
		//	m.dataf[8]  = info.sheenTint;
		//	m.dataf[9]  = info.clearCoat;
		//	m.dataf[10] = info.clearCoatGloss;
		//	m.dataf[11] = info.transmission;
		//	m.dataf[12] = info.bumpMap;

		//	m.datav[0] = vec4(info.color, -1);

		//	return m;
		//}

		std::string toString()
		{
			switch(type)
			{
				case MATERIAL_TYPE_DIFFUSE:
					return std::string("Material{type=DIFFUSE")+
						", KdTexture="+std::to_string(datai[0])+
						", sigmaTexture="+std::to_string(datai[1])+
						", Kd="+datav[0].toString()+
						", sigma="+std::to_string(dataf[0])+"}";
				case MATERIAL_TYPE_UBER:
					return std::string("Material{type=UBER")+
						", albedoTexture="+std::to_string(datai[0])+"}";
						", metallicTexture="+std::to_string(datai[1])+"}";
						", roughnessTexture="+std::to_string(datai[2])+"}";
						", aoTexture="+std::to_string(datai[3])+"}";
						", albedo="+datav[0].toString()+"}";
						", metallic="+std::to_string(dataf[0])+"}";
						", roughness="+std::to_string(dataf[1])+"}";
						", ao="+std::to_string(dataf[2])+"}";
				default:
					return std::string("Material{type=")+std::to_string(type)+"}";
			}
		}
	};
}

#endif// ATTA_GRAPHICS_CORE_MATERIAL_H
