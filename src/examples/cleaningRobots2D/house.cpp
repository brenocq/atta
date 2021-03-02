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
#include "simulator/helpers/log.h"
#include <thread>
#include <sstream>

House::House():
	_floorSize(8)
{
	//----- Allocate floorData buffer
	floorData.resize(_floorSize*_floorSize*10000, 255);// Squares of 1x1cm
	_floorTexture.resize(_floorSize*_floorSize*10000*4);

	for(unsigned i=0; i<floorData.size(); i++)
	{
		uint8_t data = floorData[i];
		_floorTexture[i*4+0] = (255-data)*0.5+126;
		_floorTexture[i*4+1] = (255-data)*0.5+126;
		_floorTexture[i*4+2] = (255-data)*0.5+126;
		_floorTexture[i*4+3] = 255;
	}

	//----- Floor
	atta::Plane::CreateInfo groundInfo {
		.name = "Floor",
		.size = {_floorSize,_floorSize},
		.mass = 0.0f,
		.material = atta::Material::diffuse({
			// Create diffuse texture from floorTexture buffer (RGBA unsigned byte (from 0 to 255))
			.kdTexture = (_floorTextureId = atta::Texture::fromBuffer(_floorTexture.data(), _floorSize*100, _floorSize*100, atta::Texture::FORMAT_RGBA_UBYTE)), 
			.sigma=10
		})
	};
	_objects.push_back(std::make_shared<atta::Plane>(groundInfo));

	//----- Left Wall
	atta::Box::CreateInfo boxInfo {
		.name = "Left wall",
		.position = {-_floorSize/2,1.5,0},
		.rotation = {0,0,0},
		.scale = {.15,3,_floorSize+0.15},
		.mass = 0.0f,
		.material = atta::Material::diffuse({.kd={.8,.8,.8}, .sigma=20})
	};
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));

	//----- Right Wall
	boxInfo.name = "Right wall";
	boxInfo.position = {4,1.5,0};
	boxInfo.scale = {.15,3,_floorSize+0.15};
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));

	//----- Top Wall
	boxInfo.name = "Top wall";
	boxInfo.position = {0,1.5,-4};
	boxInfo.scale = {_floorSize-.15,3,.15};
	_objects.push_back(std::make_shared<atta::Box>(boxInfo));

	//----- Bottom Wall
	boxInfo.name = "Bottom wall";
	boxInfo.position = {0,1.5,4};
	boxInfo.scale = {_floorSize-0.15,3,.15};
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
	dlInfo.radiance = {.5, .5, .5};
	dlInfo.direction = {1, 1, 1};
	_objects.push_back(std::make_shared<atta::DistantLight>(dlInfo));

	atta::InfiniteLight::CreateInfo ilInfo {
		.position = {0, 0, 0},
		.rotation = {atta::radians(-90), 0, 0},
		//.texture = atta::Texture::fromFile("14-Hamarikyu_Bridge_B_3k.hdr"),
		.texture = atta::Texture::fromFile("WinterForest_Ref.hdr"),
	};
	_objects.push_back(std::make_shared<atta::InfiniteLight>(ilInfo));
}

House::~House()
{

}

void House::writeFloorDataToTexture()
{
	for(unsigned i=0; i<floorData.size(); i++)
	{
		uint8_t data = floorData[i];
		_floorTexture[i*4+0] = (255-data)*0.5+126;
		_floorTexture[i*4+1] = (255-data)*0.5+126;
		_floorTexture[i*4+2] = (255-data)*0.5+126;
		_floorTexture[i*4+3] = 255;
	}

	atta::Texture::updateTexture(_floorTextureId);
}
