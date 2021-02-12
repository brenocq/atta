//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// house.cpp
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "house.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/graphics/core/texture.h"
#include "simulator/math/math.h"

House::House()
{
	float floorSize = 10;
	//----- Allocate floorData buffer
	_floorData.resize(floorSize*floorSize*10000);//10x10m floor -> squares of 10x10cm
	_floorTexture.resize(floorSize*floorSize*10000);

	//----- Floor
	atta::Plane::CreateInfo groundInfo {
		.name = "Floor",
		.size = {floorSize,floorSize},
		.mass = 0.0f,
		.material = {
			.albedo = atta::vec3(1,1,1),
			// Create albedo texture from floorData buffer and defines function to map from uint8_t to the texture color
			.albedoIndex = atta::Texture::fromBuffer(_floorTexture.data(), floorSize*100, floorSize*100)
		}
	};
	_objects.push_back(std::make_shared<atta::Plane>(groundInfo));

	//----- Left Wall
	atta::Box::CreateInfo boxInfo {
		.name = "Left wall",
		.position = {-5,1.5,0},
		.rotation = {0,0,0},
		.scale = {.15,3,10.15},
		.mass = 0.0f,
		.material = {
			.albedo = {0,0,0}
		}
	};
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));

	//----- Right Wall
	boxInfo.name = "Right wall";
	boxInfo.position = {5,1.5,0};
	boxInfo.scale = {.15,3,10.15};
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));

	//----- Top Wall
	boxInfo.name = "Top wall";
	boxInfo.position = {0,1.5,-5};
	boxInfo.scale = {9.85,3,.15};
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));

	//----- Bottom Wall
	boxInfo.name = "Bottom wall";
	boxInfo.position = {0,1.5,5};
	boxInfo.scale = {9.85,3,.15};
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));
}

House::~House()
{

}

void House::writeFloorDataToTexture()
{
	for(unsigned i=0; i<_floorTexture.size(); i++)
	{
		uint8_t data = _floorData[i];
		_floorTexture[i] = atta::vec4(data*0.0039, data*0.0039, 0, 1);
	}
}
