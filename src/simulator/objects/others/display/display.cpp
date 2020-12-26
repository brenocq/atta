//--------------------------------------------------
// Robot Simulator
// display.cpp
// Date: 2020-10-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "display.h"
#include "simulator/objects/basics/plane.h"
#include "simulator/physics/constraints/hingeConstraint.h"

Display::Display(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, DisplayInfo info):
	Object(name, position, rotation, scale), _info(info), _textureIndex(-1), _materialIndex(-1)
{
	_type = "Display";
	_model = new Model("plane");
	_bodyPhysics = new Body(&_position, &_orientation, 0.2f);

	_buffer = std::vector<uint8_t>(_info.width*_info.height*3);

	/*for(int x=0; x<_info.width; x++)
	{
		for(int y=0; y<_info.height; y++)
		{
			bool border = (x<20 || x>_info.width-20) || (y<20 || y>_info.height-20);
			_buffer[y*_info.width*3 + x*3+0] = border?255:0;
			_buffer[y*_info.width*3 + x*3+1] = border?255:0;
			_buffer[y*_info.width*3 + x*3+2] = border?255:0;
		}
	}*/
}

Display::~Display()
{

}
