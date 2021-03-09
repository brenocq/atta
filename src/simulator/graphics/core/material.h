//--------------------------------------------------
// Atta Graphics
// material.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_MATERIAL_H
#define ATTA_GRAPHICS_MATERIAL_H

#include "simulator/math/math.h"
#include "simulator/helpers/log.h"

namespace atta
{
	struct Material
	{
		alignas(4)	unsigned type[1] = {MATERIAL_TYPE_NONE};
		alignas(4)	int datai[16];
		alignas(4)	float dataf[16];
		alignas(16)	vec4 datav[2];

		enum Type
		{
			MATERIAL_TYPE_NONE = 0,
			MATERIAL_TYPE_DIFFUSE,
			MATERIAL_TYPE_MIRROR,
			MATERIAL_TYPE_METAL,
			MATERIAL_TYPE_DISNEY,
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
			float roughness = 0.5f;
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

		std::string toString()
		{
			switch(type[0])
			{
				case MATERIAL_TYPE_DIFFUSE:
					return std::string("Material{type=DIFFUSE")+
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

#endif// ATTA_GRAPHICS_MATERIAL_H
