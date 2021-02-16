//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// house.cpp
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "house.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/objects/lights/lights.h"
#include "simulator/graphics/core/texture.h"
#include "simulator/math/math.h"

House::House()
{
	float floorSize = 10;
	//----- Allocate floorData buffer
	floorData.resize(floorSize*floorSize*10000, 255);//10x10m floor -> squares of 10x10cm
	_floorTexture.resize(floorSize*floorSize*10000*4);

	for(unsigned i=0; i<floorData.size(); i++)
	{
		uint8_t data = floorData[i];
		_floorTexture[i*4] = data*0.8;
		_floorTexture[i*4+1] = data*0.75;
		_floorTexture[i*4+2] = 0;
		_floorTexture[i*4+3] = 255;
	}

	//----- Left Wall
	atta::Box::CreateInfo boxInfo {
		.name = "Left wall",
		.position = {-5,1.5,0},
		.rotation = {0,0,0},
		.scale = {.15,3,10.15},
		.mass = 0.0f,
		.material = atta::Material::diffuse({.1,.5,.7}, 20)
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

	//----- Floor
	//atta::Plane::CreateInfo groundInfo {
	//	.name = "Floor",
	//	.size = {floorSize,floorSize},
	//	.mass = 0.0f,
	//	.material = {
	//		.albedo = atta::vec3(.3,.4,.8),
	//		//.albedoIndex = (_floorTextureId = atta::Texture::fromBuffer(_floorTexture.data(), floorSize*100, floorSize*100, atta::Texture::BUFFER_TYPE_BYTE_4)),
	//		// Create albedo texture from floorData buffer and defines function to map from uint8_t to the texture color
	//	}
	//};
	//_objects.push_back(std::make_shared<atta::Plane>(groundInfo));

	boxInfo.name = "Floor";
	boxInfo.position = {0,-.01,0};
	boxInfo.scale = {10,.01,10};
	boxInfo.material = atta::Material::diffuse({.7,.7,.4}, 10);
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));

	//----- Lights -----//
	atta::PointLight::CreateInfo plInfo = {};
	plInfo.position = {-2, .1, -2};
	plInfo.intensity = {0, .3, .3};
	_objects.push_back(std::make_shared<atta::PointLight>(plInfo));

	atta::SpotLight::CreateInfo slInfo = {};
	slInfo.position = {-1, 2, -1};
	slInfo.direction = {1, -1, 1};
	slInfo.intensity = {10, 10, 10};
	slInfo.maxAngle = 60.f;
	slInfo.falloffStartAngle = 30.f;
	_objects.push_back(std::make_shared<atta::SpotLight>(slInfo));

	atta::DistantLight::CreateInfo dlInfo = {};
	dlInfo.radiance = {.3, .3, .3};
	dlInfo.direction = {1, 1, 1};
	_objects.push_back(std::make_shared<atta::DistantLight>(dlInfo));
}

House::~House()
{

}

void House::writeFloorDataToTexture()
{
	for(unsigned i=0; i<floorData.size(); i++)
	{
		uint8_t data = floorData[i];
		_floorTexture[i*4] = data*0.8;
		_floorTexture[i*4+1] = data*0.75;
		_floorTexture[i*4+2] = 0;
		_floorTexture[i*4+3] = 255;
	}

	atta::Texture::updateTexture(_floorTextureId);
}
