//--------------------------------------------------
// Atta Materials Demo Example
// materialsDemo.cpp
// Date: 2021-02-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "materialsDemo.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/objects/lights/lights.h"
#include "simulator/graphics/core/texture.h"

MaterialsDemo::MaterialsDemo()
{
	_attaCreateInfo.dimensionMode = atta::Atta::DIM_MODE_3D;
	_attaCreateInfo.physicsMode = atta::Atta::PHY_MODE_DISABLED;
	_attaCreateInfo.createWindow = true;

	std::vector<std::shared_ptr<atta::Object>> objects;

	atta::Box::CreateInfo floorInfo {
		.name = "Floor",
		.position = {0,-.01,0},
		.scale = {10,.01,10},
		.mass = 0.0f,
		.material = atta::Material::diffuse({1,1,1}, 10)
	};
	objects.push_back(std::make_shared<atta::Box>(floorInfo));

	atta::InfiniteLight::CreateInfo ilInfo {
		.position = {0, 0, 0},
		.rotation = {atta::radians(-90), 0, 0},
		//.texture = atta::Texture::fromFile("14-Hamarikyu_Bridge_B_3k.hdr"),
		.texture = atta::Texture::fromFile("WinterForest_Ref.hdr"),
	};
	objects.push_back(std::make_shared<atta::InfiniteLight>(ilInfo));

	atta::Sphere::CreateInfo sphereInfo {
		.name = "Sphere 1",
		.position = {-1,1,0},
		.radius = 1.0f,
		.mass = 0.0f,
		.material = atta::Material::diffuse({.3,.3,1}, 10)
	};
	objects.push_back(std::make_shared<atta::Sphere>(sphereInfo));

	atta::Box::CreateInfo boxInfo {
		.name = "Box1",
		.position = {0,3,0},
		.rotation = {0,0,0},
		.scale = {2,2,2},
		.mass = 0.0f,
		.material = atta::Material::diffuse({.3,.7,.3}, 10)
	};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));

	_attaCreateInfo.objects = objects;
}

MaterialsDemo::~MaterialsDemo()
{

}
