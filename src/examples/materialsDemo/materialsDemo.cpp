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

	atta::DistantLight::CreateInfo dlInfo = {};
	dlInfo.radiance = {.5, .5, .5};
	dlInfo.direction = {1, 1, 1};
	objects.push_back(std::make_shared<atta::DistantLight>(dlInfo));

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
		.material = atta::Material::diffuse({.3,.3,1}, 30)
	};
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	bunny.position = {-2,0,0};
	bunny.material = atta::Material::mirror(atta::vec3(1,1,1));
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	bunny.position = {2,0,0};
	bunny.material = atta::Material::metal({
		// Au
		.eta = {.2,1.4,1.8},
		.k = {4.3,2.8,1.8},
		.roughness = 0.1
	});
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	bunny.position = {2,2,0};
	bunny.material = atta::Material::disney({
		.color = atta::vec3(1,0,1),
		.metallic = 0.0f,
		.eta = 1.5f,
		.roughness = 0.5f,
		.specularTint = 1.0f,
		.anisotropic = 0.0f,
		.sheen = 0.0f,
		.sheenTint = 0.0f,
		.clearCoat = 0.0f,
		.clearCoatGloss = 0.0f,
		.specularTrans = 0.0f,
		.scatterDistance = atta::vec3(0,0,0),
		.flatness = 0.0f,
		.diffTrans = 0.0f,
		.bumpMap = 0.0f,
		.thin = false,
	});
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	bunny.position = {-2,2,0};
	bunny.material = atta::Material::disney({
		.color = atta::vec3(0,.2,0),
		.metallic = 1.0f,
		.eta = 0.7f,
		.roughness = 0.5f,
		.specularTint = 0.0f,
		.anisotropic = 0.0f,
		.sheen = 0.0f,
		.sheenTint = 0.0f,
		.clearCoat = 1.0f,
		.clearCoatGloss = 0.9f,
		.specularTrans = 0.0f,
		.scatterDistance = atta::vec3(0,0,0),
		.flatness = 0.0f,
		.diffTrans = 0.0f,
		.bumpMap = 0.0f,
		.thin = false,
	});
	objects.push_back(std::make_shared<atta::ImportedObject>(bunny));

	//atta::ImportedObject::CreateInfo nut {
	//	.name = "NUT",
	//	.fileName = "nut_LOW/nut_LOW.obj",
	//	.position = {0,3,0},
	//	.scale = {1.5,1.5,1.5},
	//	.mass = 0.0f,
	//	.material = atta::Material::metal({
	//			// Al
	//			//.kTexture = atta::Texture::fromFile("col.jpg"),
	//			//.roughnessTexture = atta::Texture::fromFile("spec.jpg"),
	//			.eta = {2,1.2,0.44},
	//			.k = {6,5,4},
	//			.roughness = .5
	//		}),
	//};
	//objects.push_back(std::make_shared<atta::ImportedObject>(nut));

	_attaCreateInfo.objects = objects;
}

MaterialsDemo::~MaterialsDemo()
{

}
