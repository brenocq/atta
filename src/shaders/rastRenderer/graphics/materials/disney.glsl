//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// disney.glsl
// Date: 2021-03-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_MATERIALS_DISNEY_GLSL
#define ATTA_RAST_MATERIALS_DISNEY_GLSL
#include "base.glsl"

// References
// [1] https://blog.selfshadow.com/publications/s2015-shading-course/burley/s2015_pbs_disney_bsdf_notes.pdf
// [2] https://github.com/knightcrawler25/GLSL-PathTracer/blob/master/src/shaders/common/disney.glsl
// [3] https://github.com/mmp/pbrt-v3/blob/master/src/materials/disney.cpp
// [4] https://github.com/wdas/brdf/blob/main/src/brdfs/disney.brdf
// [5] http://simon-kallweit.me/rendercompo2015/report/


vec3 Material_Disney_evalDielectricReflection(
		vec3 color, float roughness, float eta, 
		vec3 wi, vec3 wo, vec3 h, 
		vec3 n)
{
    float F = DielectricFresnel(dot(wo, h), eta);
    float D = GTR2(dot(n, h), roughness);

    //pdf = D * dot(N, H) * F / (4.0 * dot(V, H));

    float G = SmithG_GGX(abs(dot(n, wi)), roughness) * SmithG_GGX(dot(n, wo), roughness);
    return color * F * D * G;
}

vec3 Material_Disney_evalSpecular(
		float roughness, float anisotropic, vec3 colorSpec0, 
		vec3 wi, vec3 wo, vec3 h,
		vec3 n, vec3 ss, vec3 ts /*Shading orthonormal base*/)
		
{
	float aspect = sqrt(1-anisotropic*.9);
	float ax = max(0.001f, sqr(roughness)/aspect);
	float ay = max(0.001f, sqr(roughness)*aspect);
    float D = GTR2_aniso(dot(n, h), dot(h, ss), dot(h, ts), ax, ay);
    //pdf = D * dot(n, h) / (4.0 * dot(wo, h));

    float FH = SchlickFresnel(dot(wi, h));
    vec3 F = mix(colorSpec0, vec3(1.0), FH);
    float G = SmithG_GGX_aniso(dot(n, wi), dot(wi, ss), dot(wi, ts), ax, ay);
    G *= SmithG_GGX_aniso(dot(n, wo), dot(wo, ss), dot(wo, ts), ax, ay);
    return F * D * G;
}

vec3 Material_Disney_computeColor(Material material, vec3 wi, vec3 wo)
{
	vec2 uv = inTexCoord;
	vec3 brdf = vec3(0,0,0);
	vec3 btdf = vec3(0,0,0);

	// Parameters
	bool thin = material.datai[12]==1;
	float metallic = material.datai[1]!=-1 ? texture(textures[material.datai[1]], uv).x : material.dataf[0];
	float eta = material.datai[2]!=-1 ? texture(textures[material.datai[2]], uv).x : material.dataf[1];
	float roughness = material.datai[3]!=-1 ? texture(textures[material.datai[3]], uv).x : material.dataf[2];
	float specTint = material.datai[4]!=-1 ? texture(textures[material.datai[4]], uv).x : material.dataf[3];
	float anisotropic = material.datai[5]!=-1 ? texture(textures[material.datai[5]], uv).x : material.dataf[4];
	float sheen = material.datai[6]!=-1 ? texture(textures[material.datai[6]], uv).x : material.dataf[5];
	float sheenTint = material.datai[7]!=-1 ? texture(textures[material.datai[7]], uv).x : material.dataf[6];
	float clearCoat = material.datai[8]!=-1 ? texture(textures[material.datai[8]], uv).x : material.dataf[7];
	float clearCoatGloss = material.datai[9]!=-1 ? texture(textures[material.datai[9]], uv).x : material.dataf[8];
	float specTrans = material.datai[10]!=-1 ? texture(textures[material.datai[10]], uv).x : material.dataf[9];
	float flatness = material.datai[13]!=-1 ? texture(textures[material.datai[13]], uv).x : material.dataf[10];
	float diffTrans = material.datai[14]!=-1 ? texture(textures[material.datai[14]], uv).x : material.dataf[11];
	vec3 color = material.datai[0]!=-1 ? texture(textures[material.datai[0]], uv).xyz : material.datav[0].xyz;
	vec3 scatterDistance = material.datai[10]!=-1 ? texture(textures[material.datai[10]], uv).xyz : material.datav[1].xyz;

	vec3 h;
	vec3 n = normalize(inNormal);
	// TODO calculate
	vec3 ss;// primary shading tangent (tangent)
	vec3 ts;// secondary shading tangent (bitangent)

	if(n.x<=n.z && n.x<=n.y)
		ss = cross(n, vec3(1,0,0));
	else if(n.y<n.x && n.y<=n.z)
		ss = cross(n, vec3(0,1,0));
	else
		ss = cross(n, vec3(0,0,1));
	ts = cross(ss, n);

	// Half-vector from normal and incident direction (light direction)
	if(dot(n,wi)<0)
		h = normalize(wi*(1.0f/eta)+wo);// Transmission
	else
		h = normalize(wo+wi);

	if(dot(n,h)<0.0)
		h*=-1;

	// Weights
	float diffuseWeight = (1.0f-metallic)*(1.0f-specTrans);
    float transWeight = (1.0f-metallic)*specTrans;
		
	// TODO Dieletric reflection/refraction
	
	if(transWeight>0.0f)
	{
		// Transmission
		if(dot(n,wo)<=0.0f)
		{
			// TODO
		}
	}
	
	if(transWeight<1.0f)
	{
		if(dot(n,wo)>=0.0f)
		{

			float lum = lum_y(color);// Luminance approximation from RGB
			vec3 colorTint = lum>0 ? (color/lum) : vec3(1,1,1);
			vec3 colorSpec0 = mix(schlickR0FromEta(eta)*mix(vec3(1,1,1), colorTint, specTint), color, metallic);
			vec3 colorSheen = sheen>0? mix(vec3(1,1,1), colorTint, sheenTint) : vec3(0,0,0);

			// Dieletric reflection
			brdf += Material_Disney_evalDielectricReflection(color, roughness, eta, wi, wo, h, n);

			// TODO Diffuse
			//brdf += evalDiffuse(material, colorSheen, wi, wo, n, h);

			// TODO Specular
			brdf += Material_Disney_evalSpecular(roughness, anisotropic, colorSpec0, wi, wo, h, n, ss, ts);

			// TODO clearcoat

			
		}
		else
		{

		}
	}

	return brdf+btdf;
}

#endif// ATTA_RAST_MATERIALS_DISNEY_GLSL
