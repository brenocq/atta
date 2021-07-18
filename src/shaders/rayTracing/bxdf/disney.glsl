//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disney.glsl
// Date: 2021-07-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_GLSL
#define BXDF_DISNEY_GLSL
// BXDF_TYPE_DISNEY
// - dataf
// 		[0] metallic
// 		[1] roughness
// 		[2] subsurface
// 		[3] specular
// 		[4] specular tint
// 		[5] eta
// 		[6] anisotropic
// 		[7] sheen
// 		[8] sheen tint
// 		[9] clear coat
// 		[10] clear coat gloss
// 		[11] transmission
// - datav
// 		[0] color

//-----------------------------------//
//---------- Aux functions ----------//
//-----------------------------------//
float Fr(float VDotN, float etaI, float etaT)
{
    float SinThetaT2 = sqr(etaI/etaT)*(1.0f-VDotN*VDotN);
    
    // total internal reflection    
    if (SinThetaT2 > 1.0f) 
        return 1.0f;       

    float LDotN = sqrt(1.0f-SinThetaT2); 

    // todo: reformulate to remove this division
    float eta = etaT/etaI; 

    float r1 = (VDotN - eta*LDotN)/(VDotN + eta*LDotN); 
    float r2 = (LDotN - eta*VDotN)/(LDotN + eta*VDotN); 

    return 0.5f*(sqr(r1) + sqr(r2));
}

bool refractEta(vec3 wo, vec3 N, float eta, inout vec3 wi)
{
    float cosThetaI = dot(N, wi);
    float sin2ThetaI = max(0.0f, float(1.0f - cosThetaI * cosThetaI));
    float sin2ThetaT = eta * eta * sin2ThetaI;

    // total internal reflection
    if (sin2ThetaT >= 1)
        return false;

    float cosThetaT = sqrt(1.0f - sin2ThetaT);
    wi = eta * -wi + (eta * cosThetaI - cosThetaT) * N;
    return true;
}

float GTR1(float NDotH, float a)
{
    if (a >= 1) return invPi;
    float a2 = a*a;
    float t = 1 + (a2-1)*NDotH*NDotH;
    return (a2-1) / (pi*log(a2)*t);
}

float GTR2(float NDotH, float a)
{
    float a2 = a*a;
    float t = 1.0f + (a2-1.0f)*NDotH*NDotH;
    return a2/(pi * t*t);
}

float SmithGGX(float NDotv, float alphaG)
{
    float a = alphaG*alphaG;
    float b = NDotv*NDotv;
    return 1/(NDotv + sqrt(a + b - a*b));
}

float SchlickFresnel(float u)
{
    float m = clamp(1-u, 0.0f, 1.0f);
    float m2 = m*m;
    return m2*m2*m;
}

//-----------------------------------//
//--------- BXDF functions ----------//
//-----------------------------------//
vec3 BXDF_Disney_f(vec3 wo, vec3 wi, BXDF bxdf)
{
	// Based on https://github.com/mmacklin/tinsel/blob/master/src/disney.h
	vec3 color = bxdf.datav[0];
	float metallic = bxdf.dataf[0];
	float roughness = bxdf.dataf[1];
	float subsurface = bxdf.dataf[2];
	float specularTint = bxdf.dataf[4];
	float eta = bxdf.dataf[5];
	float clearcoat = bxdf.dataf[9];
	float clearcoatGloss = bxdf.dataf[10];
	float transmission = bxdf.dataf[11];

	float dotWi = wi.z;
	float dotWo = wo.z;

	vec3 H = normalize(wi+wo);

	float dotH = H.z;
	float wiDotH = dot(wi, H);

	vec3 Cdlin = color;
	float Cdlum = lum_y(Cdlin);// Luminance approximation

	vec3 Ctint = Cdlum>0.0f ? Cdlin/Cdlum : vec3(1.0f);// Normalize luminance to isolate hue+sat
	vec3 Cspec0 = mix(schlickR0FromEta(eta)*mix(vec3(1.0f), Ctint, specularTint), Cdlin, metallic);

	vec3 bsdf = vec3(0.0f, 0.0f, 0.0f);
	vec3 brdf = vec3(0.0f, 0.0f, 0.0f);

	if(transmission>0.0f)
	{
		float etaI;
		float etaO;
		if(ray.eta == 1.0f)
		{
			etaI = 1.0f;
			etaO = eta;
		}
		else
		{
			etaI = eta;
			etaO = 1.0f;
		}

		// Evaluate BSDF
		if(dotWi <= 0)
		{
			// Transmission fresnel
			float F = Fr(dotWi, etaI, etaO);
			float bsdfVal = transmission*(1.0f-F)/abs(dotWi)*(1.0f-metallic);
			bsdf = vec3(bsdfVal, bsdfVal, bsdfVal);
		}       
		else    
		{
			// Specular lobe
			float a = max(0.001f, roughness);
			float Ds = GTR2(dotH, a);

			// Fresnel term with the microfacet normal
			float FH = Fr(wiDotH, etaI, etaO);

			vec3 Fs = mix(Cspec0, vec3(1.0f), FH);
			float roughg = a;
			float Gs = SmithGGX(dotWo, roughg)*SmithGGX(dotWi, roughg);

			bsdf = Gs*Fs*Ds;
		}
	}

	if(transmission<1.0f)
    {
        // Evaluate BRDF
        if(dotWi <= 0)
        {
            if(subsurface > 0)
            {
                // take sqrt to account for entry/exit of the ray through the medium
                // this ensures transmitted light corresponds to the diffuse model
                vec3 s = vec3(sqrt(color.x), sqrt(color.y), sqrt(color.z));

                float FL = SchlickFresnel(abs(dotWi)), FV = SchlickFresnel(dotWo);
                float Fd = (1.0f-0.5f*FL)*(1.0f-0.5f*FV);

                brdf = invPi*s*subsurface*Fd*(1.0f-metallic);
            }
        }
        else
        {
            // Specular
            float a = max(0.001f, roughness);
            float Ds = GTR2(dotH, a);

            // Fresnel term with the microfacet normal
            float FH = SchlickFresnel(wiDotH);

            vec3 Fs = mix(Cspec0, vec3(1), FH);
            float roughg = a;
            float Gs = SmithGGX(dotWo, roughg)*SmithGGX(dotWi, roughg);

            // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
            // and mix in diffuse retro-reflection based on roughness
            float FL = SchlickFresnel(dotWi), FV = SchlickFresnel(dotWo);
            float Fd90 = 0.5 + 2.0f * wiDotH*wiDotH * roughness;
            float Fd = mix(1.0f, Fd90, FL) * mix(1.0f, Fd90, FV);

            // Based on Hanrahan-Krueger BSDF approximation of isotrokPic bssrdf
            // 1.25 scale is used to (roughly) preserve albedo
            // Fss90 used to "flatten" retroreflection based on roughness
            //float Fss90 = LDotH*LDotH*mat.roughness;
            //float Fss = Lerp(1.0f, Fss90, FL) * Lerp(1.0f, Fss90, FV);
            //float ss = 1.25 * (Fss * (1.0f / (NDotL + NDotV) - .5) + .5);

            // clearcoat (ior = 1.5 -> F0 = 0.04)
            float Dr = GTR1(dotH, mix(.1,.001, clearcoatGloss));
            float Fc = mix(0.04f, 1.0f, FH);
            float Gr = SmithGGX(dotWi, .25) * SmithGGX(dotWo, .25);

            /*
            // sheen
            Vec3 Fsheen = FH * mat.sheen * Csheen;
            Vec3 out = ((1/kPi) * Lerp(Fd, ss, mat.subsurface)*Cdlin + Fsheen)
				 * (1-mat.metallic)*(1.0f-mat.transmission)
				 + Gs*Fs*Ds + .25*mat.clearcoat*Gr*Fr*Dr;
            */

			brdf = invPi*Fd*Cdlin*(1.0f-metallic)*(1.0f-subsurface) + Gs*Fs*Ds + clearcoat*Gr*Fc*Dr;
        }
    }

    return mix(brdf, bsdf, transmission);
}

float BXDF_Disney_pdf(vec3 wo, vec3 wi, BXDF bxdf)
{
	float transmission = bxdf.dataf[11];
    float eta = bxdf.dataf[5];
    float subsurface = bxdf.dataf[2];
    float roughness = bxdf.dataf[1];

	float etaI;
	float etaO;
	if(ray.eta == 1.0f)
	{
		etaI = 1.0f;
		etaO = eta;
	}
	else
	{
		etaI = eta;
		etaO = 1.0f;
	}

     if(wi.z <= 0.0f)
     {
         float bsdfPdf = 0.0f;
         float brdfPdf = inv2Pi*subsurface*0.5f;
         return mix(brdfPdf, bsdfPdf, transmission);
     }
     else
     {
         float F = Fr(wo.z, etaI, etaO);
         float a = max(0.001f, roughness);

         vec3 Half = normalize(wi+wo);
         float cosThetaHalf = abs(Half.z);
         float pdfHalf = GTR2(cosThetaHalf, a)*cosThetaHalf;

         // calculate pdf for each method given outgoing light vector
         float pdfSpec = 0.25f*pdfHalf/max(1.e-6f, dot(wi, Half));
         float pdfDiff = abs(wi.z)*invPi*(1.0f-subsurface);

         float bsdfPdf = pdfSpec*F;
         float brdfPdf = mix(pdfDiff, pdfSpec, 0.5f);

         // weight pdfs equally
         return mix(brdfPdf, bsdfPdf, transmission);
     }
}

vec3 BXDF_Disney_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, BXDF bxdf) 
{
	// Sample out ray from in ray with importance sampling
    // Base vector N U V
    // N = (0,0,1)
    vec3 U = vec3(1,0,0);
    vec3 V = vec3(0,1,0);
    vec3 N = vec3(0,0,1);

    float roughness = bxdf.dataf[1];
    float eta = bxdf.dataf[5];
    float transmission = bxdf.dataf[11];
	float subsurface = bxdf.dataf[2];

    float randomT = randomFloat(ray.seed);
    if(randomT < transmission)
    {
		float etaI;
		float etaO;
		if(ray.eta == 1.0f)
		{
			etaI = 1.0f;
			etaO = eta;
		}
		else
		{
			etaI = eta;
			etaO = 1.0f;
		}

        // Sample BSDF
        float F = Fr(wo.z, etaI, etaO);

        // Sample reflectance or transmission based on Fresnel term
        float randRT = randomFloat(ray.seed);
        if(randRT < F)
        {
            // Sample specular (better 2D sampler)
            float r1 = randomFloat(ray.seed);
            float r2 = randomFloat(ray.seed);

             const float a = max(0.001f, roughness);
             const float phiHalf = r1*2*pi;

             const float cosThetaHalf = sqrt((1.0f-r2)/(1.0f + ((a*a)-1.0f)*r2));
             const float sinThetaHalf = sqrt(max(0.0f, 1.0f-(cosThetaHalf*cosThetaHalf)));
             const float sinPhiHalf = sin(phiHalf);
             const float cosPhiHalf = cos(phiHalf);

             vec3 Half = U*sinThetaHalf*cosPhiHalf + V*sinThetaHalf*sinPhiHalf + N*cosThetaHalf;

             // ensure Half angle in same hemisphere as incoming light vector
             if(wo.z <= 0.0f)
                 Half *= -1.0f;

             wi = 2.0f*dot(wo, Half)*Half - wo;

         }
         else
		 {
			 // Sample transmission
			 float currEta = etaI/etaO;
             if(refractEta(wo, N, currEta, wi))
             {
                 pdf = (1.0f-F)*transmission;
                 return BXDF_Disney_f(wo, wi, bxdf);
             }
             else
             {
                 pdf = 0.0f;
                 return BXDF_Disney_f(wo, wi, bxdf);
             }
         }
     }
     else
     {
         float r1 = randomFloat(ray.seed);
         float r2 = randomFloat(ray.seed);
 
         // Sample BRDF
         float randomD = randomFloat(ray.seed);
         if(randomD<0.5f)
         {
             float randomS = randomFloat(ray.seed);
             // Sample diffuse
             if(randomS < subsurface)
             {
               	vec3 d = uniformSampleHemisphere(vec2(r1,r2));
 
                // Negate z coordinate to sample inside the surface
               	wi = U*d.x + wo*d.y - vec3(0,0,1)*d.z;
				// TODO flag transmitted
             }
             else
             {
               vec3 d = cosineSampleHemisphere(vec2(r1, r2));
               if(d.z<0) d.z*=-1;
               wi = d;
				// TODO flag reflected
             }
         }
         else
         {
             // Sample specular
             const float a = max(0.001f, roughness);
 
             const float phiHalf = r1*2*pi;
 
             const float cosThetaHalf = sqrt((1.0f-r2)/(1.0f + (sqr(a)-1.0f)*r2));
             const float sinThetaHalf = sqrt(max(0.0f, 1.0f-sqr(cosThetaHalf)));
             const float sinPhiHalf = sin(phiHalf);
             const float cosPhiHalf = cos(phiHalf);
 
             vec3 h = vec3(sinThetaHalf*cosPhiHalf, sinThetaHalf*sinPhiHalf, cosThetaHalf);
             // ensure half angle in same hemisphere as incoming light vector
             //if(h.z <= 0.0f)
             //    h *= -1.0f;
             //if(wo.z <= 0.0f)
             //    wo *= -1.0f;
 
             wi = reflect(wo, h);//2.0f*dot(wo, h)*h - wo;
			 if(wi.z<=0) wi *= -1;
			 // TODO flag reflected
         }
     }
 
    pdf = BXDF_Disney_pdf(wo, wi, bxdf);
    return BXDF_Disney_f(wo, wi, bxdf);
}

uint BXDF_Disney_flags()
{
	return BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_DISNEY_GLSL
