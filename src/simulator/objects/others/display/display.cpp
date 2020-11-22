//--------------------------------------------------
// Robot Simulator
// display.cpp
// Date: 2020-10-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "display.h"
#include "simulator/objects/basic/plane.h"
#include "simulator/physics/constraints/hingeConstraint.h"

Display::Display(std::string name, glm::vec3 position, glm::vec3 rotation, DisplayInfo info):
	Object(name, position, rotation, {1,1,1}), _info(info), _textureIndex(-1), _materialIndex(-1)
{
	_type = "Display";
	_model = new Model("plane");
	_physics = new ObjectPhysics(_position, _rotation, 0.2);

	buffer = std::vector<uint8_t>(_info.width*_info.height*3);

	for(int x=0; x<_info.width; x++)
	{
		for(int y=0; y<_info.height; y++)
		{
			bool border = (x<20 || x>_info.width-20) || (y<20 || y>_info.height-20);
			buffer[y*_info.width*3 + x*3+0] = border?255:0;
			buffer[y*_info.width*3 + x*3+1] = border?255:0;
			buffer[y*_info.width*3 + x*3+2] = border?255:0;
		}
	}
}

Display::~Display()
{

}
