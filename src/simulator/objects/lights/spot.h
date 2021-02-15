//--------------------------------------------------
// Robot Simulator
// spot.h
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SPOT_LIGHT_H
#define ATTA_SPOT_LIGHT_H

#include "simulator/objects/object.h"

namespace atta
{
	class SpotLight : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "SpotLight";
				vec3 position = {0,0,0};
				vec3 direction = {0,-1,0};
				vec3 intensity = {1,1,1};// Light intensity (amount of power per unit solid angle)
				float maxAngle = 50;// Max angle from the diretion
				float falloffStartAngle = 30;// Falloff start angle from the direction
			};

			SpotLight(CreateInfo info);
			~SpotLight();

			//---------- Getters ----------//
			vec3 getIntensity() const { return _intensity; }
			vec3 getDirection() const { return _direction; }
			float getFalloffStart() const { return _falloffStart; }
			float getTotalWidth() const { return _totalWidth; }

		private:
			vec3 _intensity;
			vec3 _direction;
			float _falloffStart;
			float _totalWidth;
	};
}

#endif// ATTA_SPOT_LIGHT_H
