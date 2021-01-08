//--------------------------------------------------
// Robot Simulator
// material.h
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIAL_H
#define MATERIAL_H
#include "glm.h"

struct alignas(16) Material final
{
	static Material lambertian(const glm::vec3& diffuse, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, Enum::Lambertian };
	}

	static Material metallic(const glm::vec3& diffuse, const float fuzziness, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, fuzziness, 0.0f, Enum::Metallic };
	}

	static Material dielectric(const float refractionIndex, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(0.7f, 0.7f, 1.0f, 1), textureId,  0.0f, refractionIndex, Enum::Dielectric };
	}

	static Material isotropic(const glm::vec3& diffuse, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, Enum::Isotropic };
	}

	static Material diffuseLight(const glm::vec3& diffuse, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, Enum::DiffuseLight };
	}

	enum class Enum : uint32_t
	{
		Lambertian = 0,
		Metallic = 1,
		Dielectric = 2,
		Isotropic = 3,
		DiffuseLight = 4
	};

	// Note: vec3 and vec4 gets aligned on 16 bytes in Vulkan shaders. 

	// Base material
	glm::vec4 diffuse;
	int32_t diffuseTextureId;

	// Metal fuzziness
	float fuzziness;

	// Dielectric refraction index
	float refractionIndex;

	// Which material are we dealing with
	Enum materialModel;
};
#endif// MATERIAL_H
