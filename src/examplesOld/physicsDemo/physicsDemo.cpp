//--------------------------------------------------
// Atta Physics Demo Example
// physicsDemo.cpp
// Date: 2021-03-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "physicsDemo.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/objects/lights/lights.h"
#include "simulator/graphics/core/texture.h"

namespace example
{
	PhysicsDemo::PhysicsDemo()
	{
		//---------- Atta config ----------//
		_attaCreateInfo.dimensionMode = atta::DIM_MODE_3D;
		_attaCreateInfo.physicsMode = atta::PHY_MODE_3D;
		_attaCreateInfo.createWindow = true;

		//---------- Creating Objects ----------//
		std::vector<std::shared_ptr<atta::Object>> objects;
		//----- Lighting -----//
		atta::InfiniteLight::CreateInfo ilInfo {
			.position = {0, 0, 0},
			.rotation = {atta::radians(-90), 0, 0},
			.texture = atta::Texture::fromFile("WinterForest_Ref.hdr"),
			.irradianceTexture = atta::Texture::fromFile("WinterForest_Env.hdr"),
		};
		objects.push_back(std::make_shared<atta::InfiniteLight>(ilInfo));

		atta::DistantLight::CreateInfo dlInfo = {};
		dlInfo.radiance = {.5, .5, .5};
		dlInfo.direction = {-1, 1, -1};
		objects.push_back(std::make_shared<atta::DistantLight>(dlInfo));

		//----- Objects with physics -----//
		atta::HalfSpace::CreateInfo floorInfo {
			.name = "Floor",
			.normal = {0,1,0},
			.offset = 0,
			.material = atta::Material::unrealEngine4({
				.albedo = atta::vec3(.5,.5,.5),
				.metallic = 0.0f,
				.roughness = 0.5,
				.ao = 1.0f,
			})
		};
		objects.push_back(std::make_shared<atta::HalfSpace>(floorInfo));

		atta::Box::CreateInfo mat {
			.name = "Box 2kg",
			.position = {0,1.0,0},
			.rotation = {atta::radians(45),0,0},
			.scale = {1,1,1},
			.mass = 2.0f,
			.material = atta::Material::unrealEngine4({
				.albedo = atta::vec3(1,.4,.4),
				.metallic = 0.0f,
				.roughness = 0.5,
				.ao = 1.0f,
			})
		};
		objects.push_back(std::make_shared<atta::Box>(mat));

		_attaCreateInfo.objects = objects;
	}
}
