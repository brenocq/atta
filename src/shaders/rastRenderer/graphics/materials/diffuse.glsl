//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// diffuse.glsl
// Date: 2021-03-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_MATERIALS_DIFFUSE_GLSL
#define ATTA_RAST_MATERIALS_DIFFUSE_GLSL
#include "base.glsl"

vec3 Material_Diffuse_computeColor(Material material, vec2 uv)
{
	vec3 Kd = material.datai[0]==-1 ? material.datav[0].xyz : texture(textures[material.datai[0]], uv).xyz;
	float sigma = material.datai[1]==-1 ? material.dataf[0] : texture(textures[material.datai[1]], uv).x;

	if(!isBlack(Kd))// If not black
	{
		if(sigma == 0)
		{
			//----- Lambertian -----//
			return Kd*invPi;
		}
		else
		{
			//----- Oren Nayar -----//

			// Compute A and B
			float A, B;
			float sigma2 = radians(sigma); sigma2 = sigma2*sigma2;
			A = 1.f - (sigma2/(2.f*(sigma2+0.33f)));
			B = 0.45f * sigma2/(sigma2 + 0.09f);

			vec3 wi = vec3(1,0,0);
			vec3 wo = normalize(vec3(2,2,2));

			float sinThetaI = sinTheta(wi);
			float sinThetaO = sinTheta(wo);

			// Compute cosine term of Oren–Nayar model
			float maxCos = 0;
			if(sinThetaI > 1e-4 && sinThetaO > 1e-4) {
				float sinPhiI = sinPhi(wi), cosPhiI = cosPhi(wi);
				float sinPhiO = sinPhi(wo), cosPhiO = cosPhi(wo);
				float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
				maxCos = max(0.f, dCos);
			}
			
			// Compute sine and tangent terms of Oren–Nayar model
			float sinAlpha, tanBeta;
			if(absCosTheta(wi) > absCosTheta(wo))
			{
				sinAlpha = sinThetaO;
				tanBeta = sinThetaI / absCosTheta(wi);
			}
			else
			{
				sinAlpha = sinThetaI;
				tanBeta = sinThetaO / absCosTheta(wo);
			}

			return Kd*invPi * (A + B*maxCos*sinAlpha*tanBeta) * vec3(2,2,2);
		}
	}

	return vec3(0,0,0);
}

#endif// ATTA_RAST_MATERIALS_DIFFUSE_GLSL
