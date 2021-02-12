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

	private:
		void writeFloorDataToTexture();

		std::vector<uint8_t> _floorData;
		std::vector<atta::vec4> _floorTexture;
		std::vector<std::shared_ptr<atta::Object>> _objects;
};
#endif// ATTA_EXAMPLE_CLEANING_ROBOTS_2D_HOUSE
