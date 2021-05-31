//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// fresnelSpecular.glsl    
// Date: 2021-05-31
// By Breno Cunha Queiroz  
//--------------------------------------------------
#ifndef BXDF_FRESNEL_SPECULAR_GLSL
#define BXDF_FRESNEL_SPECULAR_GLSL
#include "../base.glsl"    
#include "base.glsl"       

vec3 BXDF_FresnelSpecular_f()
{
    return vec3(0,0,0);    
} 

float BXDF_FresnelSpecular_pdf(vec3 wo, vec3 wi)
{ 
    return 0;              
}     
      
vec3 BXDF_FresnelSpecular_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
        vec3 R, vec3 T, float etaA, float etaB) 
{
    float F = FrDieletric(cosTheta(wo), etaA, etaB);
      
    if(u[0]<F)
    { 
        // Compute specular reflection for FresnelSpecular
        // Compute perfect specular reflection direction
        wi = vec3(-wo.x, -wo.y, wo.z);  
        pdf = F;
        return F*R/absCosTheta(wi);     
    } 
    else
    {
        // Compute specular transmission for FresnelSpecular
    
        // Figure out which eta is incident and which is transmitted
        bool entering = cosTheta(wo)>0; 
        float etaI = entering?etaA:etaB;
        float etaT = entering?etaB:etaA;
    
        // Compute ray direction for specular transmission
        if(!refractWi(wo, faceForward(vec3(0,0,1), wo), etaI/etaT, wi))
            return vec3(0,0,0);
    
        vec3 ft = T*(1-F);
    
        // Account for non-symmetry with transmission to different medium
        // TODO check transportmode     
        ft *= (etaI*etaI)/(etaT*etaT);  
        pdf = 1-F;         
        return ft/absCosTheta(wi);
    }
}

uint BXDF_FresnelSpecular_flags()
{
    return BXDF_FLAG_REFLECTION | BXDF_FLAG_TRANSMISSION | BXDF_FLAG_SPECULAR;
}

#endif// BXDF_FRESNEL_SPECULAR_GLSL
