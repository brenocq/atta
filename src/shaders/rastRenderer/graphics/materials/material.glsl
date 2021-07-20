//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// material.glsl
// Date: 2021-03-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_BSDF_MATERIAL_GLSL
#define ATTA_RAST_BSDF_MATERIAL_GLSL
#include "../../material.glsl"
#include "diffuse.glsl"
#include "uber.glsl"

vec3 Material_f(Material material, vec3 wi, vec3 wo)
{
	switch(material.type)
	{
		case MATERIAL_TYPE_DIFFUSE:
			return Material_Diffuse_computeColor(material, wi, wo);
		case MATERIAL_TYPE_UBER:
			return Material_Uber_computeColor(material, wi, wo);
		default:
			return vec3(1,1,1);
	}
}

vec3 Material_IBL(Material material, vec3 n, vec3 v, Light light)
{
	switch(material.type)
	{
		//case MATERIAL_TYPE_UBER:
		//	return Material_Uber_IBL(material, n, v, light);
		//case MATERIAL_TYPE_DIFFUSE:
		//	// Assumptions: 
		//	//  - material color at datav[0]
		//	//  - roughness = 0.9
		//	//  - Dielectric material
		//	//  - No specular brdf

		//	vec3 color = material.datai[0]==-1 ? material.datav[0].xyz : texture(textures[material.datai[0]], inTexCoord).xyz;
		//	//----- IBL diffuse -----//
		//	vec3 F0 = vec3(0.04); 
		//	vec3 F = fresnelSchlickRoughness(max(dot(n, v), 0.0), F0, 0.9f);
    	//	vec3 kD = 1.0 - F;
		//	vec3 wi;
		//	vec3 diffuse = kD*Light_sampleLi(light, wi)*color;
		//	return diffuse;
		default:
			return vec3(0,0,0);
	}
}

#endif// ATTA_RAST_BSDF_MATERIAL_GLSL
