//--------------------------------------------------
// Robot Simulator
// display.cpp
// Date: 2020-10-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/others/display/display.h>
#include <atta/objects/basics/plane.h>
#include <atta/physics/constraints/hingeConstraint.h>

namespace atta
{
	Display::Display(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale, info.mass}), _info(info.screenInfo), _textureIndex(-1), _materialIndex(-1)
	{
		setType("Display");
		_buffer = std::vector<uint8_t>(_info.width*_info.height*3);

		//_model = new Model("plane");
		//_bodyPhysics = new Body(&_position, &_orientation, 0.2f);


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
}
