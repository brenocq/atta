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
		.material = atta::Material::diffuse({.7,.7,.7}, 0)
	};
	objects.push_back(std::make_shared<atta::Box>(floorInfo));

	atta::InfiniteLight::CreateInfo ilInfo {
		.position = {0, 0, 0},
		.rotation = {atta::radians(-90), 0, 0},
		//.texture = atta::Texture::fromFile("14-Hamarikyu_Bridge_B_3k.hdr"),
		.texture = atta::Texture::fromFile("WinterForest_Ref.hdr"),
	};
	objects.push_back(std::make_shared<atta::InfiniteLight>(ilInfo));

	//atta::Sphere::CreateInfo sphereInfo {
	//	.name = "Sphere 1",
	//	.position = {-3,1,0},
	//	.radius = 1.0f,
	//	.mass = 0.0f,
	//	.material = atta::Material::diffuse({.3,.3,1}, 0)
	//};
	//objects.push_back(std::make_shared<atta::Sphere>(sphereInfo));

	//sphereInfo.position = {0,1,0},
	//sphereInfo.material = atta::Material::diffuse({.3,.3,1}, 40);
	//objects.push_back(std::make_shared<atta::Sphere>(sphereInfo));

	atta::ImportedObject::CreateInfo bunny {
		.name = "Bunny 1",
		.fileName = "bunny/bunny.obj",
		.position = {0,0,0},
		.scale = {10,10,10},
		.mass = 0.0f,
		.material = atta::Material::diffuse({.3,.3,1}, 0)
	};
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	bunny.position = {-2,0,0};
	bunny.material = atta::Material::mirror(atta::vec3(1,1,1));
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	bunny.position = {2,0,0};
	bunny.material = atta::Material::metal({
		// Al
		//.eta = {2,1.2,0.44},
		//.k = {6,5,4},
		// Au
		.eta = {.2,1.4,1.8},
		.k = {4.3,2.8,1.8},
		.roughness = 0.3f
	});
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	_attaCreateInfo.objects = objects;
}

MaterialsDemo::~MaterialsDemo()
{

}
