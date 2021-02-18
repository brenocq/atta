//--------------------------------------------------
// Atta Graphics
// material.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_MATERIAL_H
#define ATTA_GRAPHICS_MATERIAL_H

#include "simulator/math/math.h"

namespace atta
{
	struct Material
	{
		unsigned type[8] = {MATERIAL_TYPE_NONE};
		int datai[16];
		float dataf[16];
		vec4 datav[16];

		enum Type
		{
			MATERIAL_TYPE_NONE = 0,
			MATERIAL_TYPE_DIFFUSE,
			MATERIAL_TYPE_MIRROR,
			MATERIAL_TYPE_METAL,
		};

		static Material diffuse(vec3 Kd, float sigma)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_DIFFUSE;
			m.datav[0] = vec4(Kd,-1);
			m.dataf[0] = sigma;

			return m;
		}

		static Material mirror(vec3 R=vec3(1,1,1))
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_MIRROR;
			m.datav[0] = vec4(R,-1);

			return m;
		}

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
