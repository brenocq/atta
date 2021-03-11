//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// cleaner.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EXAMPLE_CLEANING_ROBOTS_2D_CLEANER
#define ATTA_EXAMPLE_CLEANING_ROBOTS_2D_CLEANER

#include "simulator/core/robot.h"
#include "simulator/math/math.h"

namespace example
{
	class Cleaner : public atta::Robot
	{
		public:
			Cleaner(atta::vec3 position={0,0,0}, atta::vec3 rotation={0,0,0});
			~Cleaner();

			void run(float dt);

			//---------- Getters ----------//
			float getRadius() const { return _radius; }
			atta::vec3 getPosition() const { return _position; }
			float getTheta() const { return _theta; }
			float getVel() const { return _vel; }

			//---------- Setters ----------//
			void setTheta(float theta) { _theta = theta; }

		private:
			void updateRootObject();

			float _radius;

			atta::vec3 _position;
			float _theta;// Angle in radians
			float _vel;// Velocity in m/s
	};
}

#endif// ATTA_EXAMPLE_CLEANING_ROBOTS_2D_CLEANER
