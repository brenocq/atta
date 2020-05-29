#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm.h"

struct alignas(16) Material
{
	static Material Lambertian(const glm::vec3& diffuse, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, Enum::lambertian };
	}

	static Material Metallic(const glm::vec3& diffuse, const float fuzziness, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, fuzziness, 0.0f, Enum::metallic };
	}

	static Material Dielectric(const float refractionIndex, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(0.7f, 0.7f, 1.0f, 1), textureId,  0.0f, refractionIndex, Enum::dielectric };
	}

	static Material Isotropic(const glm::vec3& diffuse, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, Enum::isotropic };
	}

	static Material DiffuseLight(const glm::vec3& diffuse, const int32_t textureId = -1)
	{
		return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, Enum::diffuseLight };
	}

	enum class Enum : uint32_t
	{
		lambertian = 0,
		metallic = 1,
		dielectric = 2,
		isotropic = 3,
		diffuseLight = 4
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
