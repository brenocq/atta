#include "random.glsl"
#include "rayPayload.glsl"

// Polynomial approximation by Christophe Schlick
float schlick(const float cosine, const float refractionIndex)
{
	float r0 = (1 - refractionIndex) / (1 + refractionIndex);
	r0 *= r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

// Lambertian
RayPayload scatterLambertian(const Material m, const vec3 direction, const vec3 normal, const vec2 texCoord, const float t, inout uint seed)
{
	const bool isScattered = dot(direction, normal) < 0;
	const vec4 texColor = m.diffuseTextureId >= 0 ? texture(TextureSamplers[m.diffuseTextureId], texCoord) : vec4(1);
	const vec4 colorAndDistance = vec4(m.diffuse.rgb * texColor.rgb, t);
	const vec4 scatter = vec4(normal + randomInUnitSphere(seed), isScattered ? 1 : 0);

	return RayPayload(colorAndDistance, scatter, seed);
}

// Metallic
RayPayload scatterMetallic(const Material m, const vec3 direction, const vec3 normal, const vec2 texCoord, const float t, inout uint seed)
{
	const vec3 reflected = reflect(direction, normal);
	const bool isScattered = dot(reflected, normal) > 0;

	const vec4 texColor = m.diffuseTextureId >= 0 ? texture(TextureSamplers[m.diffuseTextureId], texCoord) : vec4(1);
	const vec4 colorAndDistance = isScattered ? vec4(m.diffuse.rgb * texColor.rgb, t) : vec4(1, 1, 1, -1);
	const vec4 scatter = vec4(reflected + m.fuzziness*randomInUnitSphere(seed), isScattered ? 1 : 0);

	return RayPayload(colorAndDistance, scatter, seed);
}

// Dielectric
RayPayload scatterDieletric(const Material m, const vec3 direction, const vec3 normal, const vec2 texCoord, const float t, inout uint seed)
{
	const float dot = dot(direction, normal);
	const vec3 outwardNormal = dot > 0 ? -normal : normal;
	const float niOverNt = dot > 0 ? m.refractionIndex : 1 / m.refractionIndex;
	const float cosine = dot > 0 ? m.refractionIndex * dot : -dot;

	const vec3 refracted = refract(direction, outwardNormal, niOverNt);
	const float reflectProb = refracted != vec3(0) ? schlick(cosine, m.refractionIndex) : 1;

	const vec4 texColor = m.diffuseTextureId >= 0 ? texture(TextureSamplers[m.diffuseTextureId], texCoord) : vec4(1);
	
	return randomFloat(seed) < reflectProb
		? RayPayload(vec4(texColor.rgb, t), vec4(reflect(direction, normal), 1), seed)
		: RayPayload(vec4(texColor.rgb, t), vec4(refracted, 1), seed);
}

// Diffuse Light
RayPayload scatterDiffuseLight(const Material m, const float t, inout uint seed)
{
	const vec4 colorAndDistance = vec4(m.diffuse.rgb, t);
	const vec4 scatter = vec4(1, 0, 0, 0);

	return RayPayload(colorAndDistance, scatter, seed);
}

RayPayload scatter(const Material m, const vec3 direction, const vec3 normal, const vec2 texCoord, const float t, inout uint seed)
{
	const vec3 normDirection = normalize(direction);

	switch (m.materialModel)
	{
	case MaterialLambertian:
		return scatterLambertian(m, normDirection, normal, texCoord, t, seed);
	case MaterialMetallic:
		return scatterMetallic(m, normDirection, normal, texCoord, t, seed);
	case MaterialDielectric:
		return scatterDieletric(m, normDirection, normal, texCoord, t, seed);
	case MaterialDiffuseLight:
		return scatterDiffuseLight(m, t, seed);
	}
}

