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
	_attaCreateInfo.dimensionMode = atta::DIM_MODE_3D;
	_attaCreateInfo.physicsMode = atta::PHY_MODE_DISABLED;
	_attaCreateInfo.createWindow = true;

	std::vector<std::shared_ptr<atta::Object>> objects;

	atta::Box::CreateInfo floorInfo {
		.name = "Floor",
		.position = {0,-.01,0},
		.scale = {10,.01,10},
		.mass = 0.0f,
		.material = atta::Material::unrealEngine4({
			.albedo = atta::vec3(.5,.5,.5),
			.metallic = 0.0f,
			.roughness = 0.5,
			.ao = 1.0f,
		})
	};
	objects.push_back(std::make_shared<atta::Box>(floorInfo));

	atta::InfiniteLight::CreateInfo ilInfo {
		.position = {0, 0, 0},
		.rotation = {atta::radians(-90), 0, 0},
		//.texture = atta::Texture::fromFile("14-Hamarikyu_Bridge_B_3k.hdr"),
		.texture = atta::Texture::fromFile("WinterForest_Ref.hdr"),
		.irradianceTexture = atta::Texture::fromFile("WinterForest_Env.hdr"),
	};
	objects.push_back(std::make_shared<atta::InfiniteLight>(ilInfo));

	atta::DistantLight::CreateInfo dlInfo = {};
	dlInfo.radiance = {.5, .5, .5};
	dlInfo.direction = {-1, 1, -1};
	objects.push_back(std::make_shared<atta::DistantLight>(dlInfo));

	atta::PointLight::CreateInfo plInfo = {};
	plInfo.position = {-2, .4, -2};
	plInfo.intensity = {0, 15, 15};
	objects.push_back(std::make_shared<atta::PointLight>(plInfo));

	//---------- Diffuse material (Oren Nayar) ----------//
	atta::ImportedObject::CreateInfo mat {
		.name = "Diffuse material",
		.fileName = "material_ball/material_ball.obj",
		.position = {0,2.8,0},
		.rotation = {0,atta::radians(180),0},
		.scale = {.1,.1,.1},
		.mass = 0.0f,
		.material = atta::Material::diffuse({
			.kd = {.3,.3,1},
			.sigma = 30
		})
	};
	objects.push_back(std::make_shared<atta::ImportedObject>(mat));

	//---------- Diffuse material (Lambertian) ----------//
	
	//---------- Mirror material ----------//
	//mat.position = {-2,0,0};
	//mat.material = atta::Material::mirror(atta::vec3(1,1,1));
	//objects.push_back(std::make_shared<atta::ImportedObject>(mat));

	//---------- Ea4 material (Custom) ----------//
	mat.position = {2,.8,0};
	mat.material = atta::Material::unrealEngine4({
		.albedoTexture = atta::Texture::fromFile("../models/material_ball_color/material_ball_color.png"),
		.metallicTexture = atta::Texture::fromFile("../models/material_ball_metalness/material_ball_metalness.png"),
		.roughnessTexture = atta::Texture::fromFile("../models/material_ball_gloss/material_ball_gloss.png"),
		.aoTexture = atta::Texture::fromFile("../models/material_ball_ao/material_ball_ao.png"),
	});
	objects.push_back(std::make_shared<atta::ImportedObject>(mat));

	//---------- Ea4 material (Wood) ----------//
	mat.position = {0,.8,0};
	mat.material = atta::Material::unrealEngine4({
		.albedoTexture = atta::Texture::fromFile("bambooWoodSemigloss-albedo.png"),
		.metallicTexture = atta::Texture::fromFile("bambooWoodSemigloss-metallic.png"),
		.roughnessTexture = atta::Texture::fromFile("bambooWoodSemigloss-roughness.png"),
		.aoTexture = atta::Texture::fromFile("bambooWoodSemigloss-ao.png"),
	});
	objects.push_back(std::make_shared<atta::ImportedObject>(mat));
	
	//---------- Ea4 material (Rust) ----------//
	mat.position = {-2,.8,0};
	mat.material = atta::Material::unrealEngine4({
		.albedoTexture = atta::Texture::fromFile("rustedIron-albedo.png"),
		.metallicTexture = atta::Texture::fromFile("rustedIron-metallic.png"),
		.roughnessTexture = atta::Texture::fromFile("rustedIron-roughness.png"),
		.ao = 1.0
	});
	objects.push_back(std::make_shared<atta::ImportedObject>(mat));

	_attaCreateInfo.objects = objects;
}

MaterialsDemo::~MaterialsDemo()
{

}
