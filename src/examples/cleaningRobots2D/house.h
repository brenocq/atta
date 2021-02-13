//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// house.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EXAMPLE_CLEANING_ROBOTS_2D_HOUSE
#define ATTA_EXAMPLE_CLEANING_ROBOTS_2D_HOUSE

#include <memory>
#include "simulator/objects/object.h"

class House
{
	public:
		House();
		~House();

		std::vector<std::shared_ptr<atta::Object>> getObjects() const { return _objects; }
		void writeFloorDataToTexture();

		std::vector<uint8_t> floorData;// Used in the simulation to store the amount of dirt on the floor (0->clean, 255->very dirty)
	private:

		std::vector<std::shared_ptr<atta::Object>> _objects;

		// Floor texture data
		std::vector<uint8_t> _floorTexture;// Store the floor color (used to update the floor texture as the robots move)
		int _floorTextureId; // Floor albedo texture id (necessary to update the texture)
};
#endif// ATTA_EXAMPLE_CLEANING_ROBOTS_2D_HOUSE
