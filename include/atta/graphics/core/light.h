//--------------------------------------------------
// Atta Graphics
// light.h
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_CORE_LIGHT_H
#define ATTA_GRAPHICS_CORE_LIGHT_H

#include <atta/math/math.h>
#include <atta/helpers/log.h>
#include <atta/graphics/core/texture.h>

namespace atta
{
	struct Light
	{
		alignas(4) unsigned type = {LIGHT_TYPE_NONE};
		alignas(4) unsigned nSamples = 1;
		alignas(16) mat4 lightToWorld = mat4(1);
		alignas(16) mat4 worldToLight = mat4(1);
		alignas(4) unsigned datai[5];
		alignas(4) float dataf[2];
		alignas(16) vec4 datav[2];

		enum Type
		{
			LIGHT_TYPE_NONE = 0,
			LIGHT_TYPE_POINT,
			LIGHT_TYPE_SPOT,
			LIGHT_TYPE_DISTANT,
			LIGHT_TYPE_PROJECTION,
			LIGHT_TYPE_INFINITE,
			LIGHT_TYPE_GONIOMETRIC,
			LIGHT_TYPE_DIFFUSE,
		};

		static Light point(vec3 position, vec3 intensity)
		{
			Light l;
			l.type = LIGHT_TYPE_POINT;
			l.datav[0] = vec4(intensity, -1);
			l.lightToWorld = transpose(mat4(1).translate(position));
			l.worldToLight = inverse(l.lightToWorld);

			return l;
		}

		static Light spot(vec3 position, vec3 direction, vec3 intensity, float falloffStart, float totalWidth)
		{
			Light l;
			l.type = LIGHT_TYPE_SPOT;
			l.datav[0] = vec4(intensity, -1);
			l.dataf[0] = cos(radians(falloffStart));
			l.dataf[1] = cos(radians(totalWidth));

			quat q;
			q.transformVector(vec3(0,0,1), normalize(direction));
			mat4 m = posOri(position, q);
			Log::debug("Light", "$0", m.toString());
			l.lightToWorld = transpose(m);
			l.worldToLight = inverse(l.lightToWorld);

			return l;
		}

		static Light distant(vec3 radiance, vec3 direction)
		{
			Light l;
			l.type = LIGHT_TYPE_DISTANT;
			l.datav[0] = vec4(radiance, -1);
			l.datav[1] = vec4(normalize(direction), -1);
			l.lightToWorld = mat4(1);
			l.worldToLight = mat4(1);

			return l;
		}

		static Light infinite(vec3 worldCenter, quat orientation, vec3 precomputedPower, float worldRadius, int textureIndex, int pdfIndex, int irradianceMapIndex)
		{
			//Log::debug("LIGHT INF", "tex:$0 pdf:$1", textureIndex, pdfIndex);
			Light l;
			l.type = LIGHT_TYPE_INFINITE;
			l.datav[0] = vec4(worldCenter, -1);
			l.datav[1] = vec4(precomputedPower, -1);
			l.datai[0] = textureIndex;
			l.datai[1] = pdfIndex;
			l.datai[2] = irradianceMapIndex;
			l.datai[3] = textureIndex;
			l.datai[4] = Texture::fromFile("/usr/include/atta/assets/textures/attaIBL/ibl_brdf_lut.png");
			l.lightToWorld = transpose(posOri(worldCenter, orientation));
			l.worldToLight = inverse(l.lightToWorld);

			return l;
		}

		std::string toString()
		{
			switch(type)
			{
				case LIGHT_TYPE_POINT:
					return std::string("Light{type=POINT")+
						", nSamples="+std::to_string(nSamples)
						+"}";
				default:
					return std::string("Light{type=")+std::to_string(type)+"}";
			}
		}
	};
}

#endif// ATTA_GRAPHICS_CORE_LIGHT_H
