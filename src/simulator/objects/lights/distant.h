//--------------------------------------------------
// Robot Simulator
// distant.h
// Date: 2021-02-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_DISTANT_LIGHT_H
#define ATTA_DISTANT_LIGHT_H

#include "simulator/objects/object.h"

namespace atta
{
	class DistantLight : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "DistantLight";
				vec3 radiance = {1,1,1};// Emitted radiance by a disk illuminating the scene (5km radius)
				vec3 direction = {1,1,1};// Incident ray from the object being illuminated to the disk
			};

			DistantLight(CreateInfo info);
			~DistantLight();

			//---------- Getters ----------//
			vec3 getRadiance() const { return _radiance; }
			vec3 getDirection() const { return _direction; }

		private:
			vec3 _radiance;
			vec3 _direction;
	};
}

#endif// ATTA_DISTANT_LIGHT_H
