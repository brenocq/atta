//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// unrealEngine4.glsl
// Date: 2021-03-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_MATERIALS_UNREAL_ENGINE_4_GLSL
#define ATTA_RAST_MATERIALS_UNREAL_ENGINE_4_GLSL
#include "base.glsl"
#include "../lights/light.glsl"

// References
// [1] https://blog.selfshadow.com/publications/s2015-shading-course/burley/s2015_pbs_disney_bsdf_notes.pdf
// [2] https://learnopengl.com/PBR/Theory

vec3 Material_UnrealEngine4_computeColor(Material material, vec3 wi, vec3 wo)
{
	vec2 uv = inTexCoord;
	// Parameters
	float metallic = material.datai[1]!=-1 ? texture(textures[material.datai[1]], uv).x : material.dataf[0];
	float roughness = material.datai[2]!=-1 ? texture(textures[material.datai[2]], uv).x : material.dataf[1];
	float ao = material.datai[3]!=-1 ? texture(textures[material.datai[3]], uv).x : material.dataf[2];
	vec3 albedo;
	if(material.datai[0]!=-1) {
		albedo = texture(textures[material.datai[0]], uv).xyz;
		// Compile error when albedo = pow(albedo, 2.2f);
		albedo = vec3(pow(albedo.x, 2.2f), pow(albedo.y, 2.2f), pow(albedo.z, 2.2f));
	}
	else
		albedo = material.datav[0].xyz;

	vec3 v = wo;
	vec3 l = wi;
	vec3 h = normalize(v+l);
	vec3 n = normalize(inNormal);

	// Fresnel reflectance
	vec3 F0 = vec3(0.04);// Assumption that 0.04 is a good value for dielectric surfaces
	F0 = mix(F0, albedo, metallic);

	// Cook-Torrance microfacet specular shading model
	float NDF = distributionGGX(n, h, roughness);// Normal distribution function
    float G = geometrySmith(n, v, l, roughness);// Specular geometric attenuation
	vec3 F = fresnelSchlick(max(dot(h, v), 0.0), F0);
	vec3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0);
	vec3 specular = nominator/max(denominator, 0.001);

	vec3 kS = F;
	vec3 kD = vec3(1.0)-kS;
	kD *= 1.0 - metallic;

	return kD*albedo*invPi + specular;
}

vec3 Material_UnrealEngine4_IBL(Material material, vec3 n, vec3 v, Light light)
{
	vec3 wi;
	vec3 irradiance = Light_sampleLi(light, wi);

	vec2 uv = inTexCoord;
	// Parameters
	float metallic = material.datai[1]!=-1 ? texture(textures[material.datai[1]], uv).x : material.dataf[0];
	float roughness = material.datai[2]!=-1 ? texture(textures[material.datai[2]], uv).x : material.dataf[1];
	float ao = material.datai[3]!=-1 ? texture(textures[material.datai[3]], uv).x : material.dataf[2];
	vec3 albedo = material.datai[0]!=-1 ? texture(textures[material.datai[0]], uv).xyz : material.datav[0].xyz;

	// Fresnel reflectance
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	//----- IBL diffuse -----//
	vec3 F = fresnelSchlickRoughness(max(dot(n, v), 0.0), F0, roughness);
	vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    vec3 diffuse = irradiance * albedo;

	//----- IBL specular -----//
	//Split-sum approximation from pre-filter map and BRDF lut
	//vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
	// Calculate  direction
	vec3 r = reflect(-v, n);
	vec3 rLocal = normalize((light.worldToLight*vec4(r,0)).xyz);
	vec2 st = vec2(sphericalPhi(rLocal)*inv2Pi, sphericalTheta(rLocal)*invPi);
	vec3 prefilteredColor = light.datai[3]!=-1 ? textureLod(textures[light.datai[3]], st, roughness*5.0f).rgb : light.datav[2].rgb;
    vec2 brdf  = texture(textures[light.datai[4]], vec2(max(dot(n, v), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    return (kD * diffuse + specular) * ao;
}

#endif// ATTA_RAST_MATERIALS_UNREAL_ENGINE_4_GLSL
