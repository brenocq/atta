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
		};

		static Material diffuse(vec3 Kd, float sigma)
		{
			Material m;
			m.type[0] = MATERIAL_TYPE_DIFFUSE;
			m.datav[0] = vec4(Kd,-1);
			m.dataf[0] = sigma;

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
				default:
					return std::string("Material{type=")+std::to_string(type[0])+"}";
			}
		}
	};
}

#endif// ATTA_GRAPHICS_MATERIAL_H
