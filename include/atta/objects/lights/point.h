//--------------------------------------------------
// Robot Simulator
// point.h
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_LIGHTS_POINT_LIGHT_H
#define ATTA_OBJECTS_LIGHTS_POINT_LIGHT_H

#include <atta/objects/object.h>

namespace atta
{
	class PointLight : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "PointLight";
				vec3 position = {0,0,0};
				vec3 intensity = {1,1,1};// Light intensity (amount of power per unit solid angle)
			};

			PointLight(CreateInfo info);
			~PointLight();

			//---------- Getters ----------//
			vec3 getIntensity() const { return _intensity; }

		private:
			vec3 _intensity;
	};
}

#endif// ATTA_OBJECTS_LIGHTS_POINT_LIGHT_H
