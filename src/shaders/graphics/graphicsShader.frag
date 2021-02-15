#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#define PI 3.1415926538
#include "../material.glsl"

layout(binding = 1) readonly buffer MaterialArray { Material[] materials; };
//layout(binding = 2) uniform sampler2D[] textureSamplers;
//layout(binding = 3) uniform sampler2D irradianceMap;


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in flat int inMaterialIndex;
layout(location = 4) in vec3 inViewPos;

layout(location = 0) out vec4 outColor;

//const vec3 ambientColor = vec3(.1,.1,.1);
//const vec3 lightColor = vec3(.4,.4,.4);
//const vec3 lightPos = vec3(0, 10, 0);
//const vec3 specularColor = vec3(.3,.3,.3);
//const float specularAlpha = 200.0;
//const vec3 dirLightColor = vec3(.6,.6,.6);
//const vec3 dirLight = vec3(0,1,1);

const vec2 invAtan = vec2(0.15915, 0.3183);
vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}


float calculateAttenuation(vec3 pos, vec3 lightPos)
{
	return 1.0;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometrySchlickGGX(NdotV, roughness);
    float ggx1  = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main() 
{
	Material material = materials[inMaterialIndex];

	// Light
	vec3 lightPos	 = vec3(0,3,2);
	vec3 lightColor  = vec3(23.47, 21.31, 20.79);

	// Surface color
	vec3 albedo = vec3(.4, .8, .4);//material.albedo;
	float metallic = .8;//material.metallic;
	float roughness = .3;//material.roughness;
	float ao = 1;

	// Surface vectors
	vec3 N = normalize(inNormal);
	vec3 V = normalize(inViewPos - inPos);

	vec3 F0 = vec3(0.04);
	F0      = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	// Calculate contribution for each light
	for(int i=0; i<1; i++)
	{
		vec3 L = normalize(lightPos - inPos);
		vec3 H = normalize(V + L);
	  
		float distance    = length(lightPos - inPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance     = lightColor * attenuation; 

		vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
		float NDF = distributionGGX(N, H, roughness);
		float G   = geometrySmith(N, V, L, roughness);

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);

		vec3 kS = F;// Specular contribution (reflected light energy)
		vec3 kD = vec3(1.0) - kS;// Rate of reflaction
		kD *= 1.0 - metallic;

		//float PI = 3.14159265359;
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
	vec3 kD = 1.0 - kS;
	vec2 uvIrr = sampleSphericalMap(normalize(N));
	vec3 irradiance = vec3(1,1,1);//texture(irradianceMap, uvIrr).rgb;
	vec3 diffuse    = irradiance * albedo;
	vec3 ambient = (kD*diffuse) * ao;
	vec3 color   = ambient + Lo;  


	//int textureId = -1;
	//if(fragMaterialIndex >= 0)
	//	textureId = materials[fragMaterialIndex].diffuseTextureId;

	//const vec3 l = normalize(lightPos - fragPos);
	//const vec3 n = normalize(fragNormal);
	//const vec3 v = normalize(viewPos - fragPos);
	//const vec3 h = normalize(l+v);
	//vec3 S = specularColor*pow(clamp(dot(n, h), 0.0, 1.0), specularAlpha);
	//if(dot(n, lightPos)<=0)
	//	S = S*0;
	//const vec3 diff = fragColor*clamp(dot(n,  l), 0.0, 1.0);
	//const vec3 dir = fragColor*clamp(dot(n,  dirLight), 0.0, 1.0);
	//const vec3 directColor = lightColor*(diff+S);
	//const vec3 directionalColor = dirLightColor*dir;
	//
	//vec3 color = (ambientColor+directColor+directionalColor);
	//if(textureId >= 0)
	//{
	//	color *= texture(textureSamplers[textureId], fragTexCoord).rgb;
	//}

	//vec3 color = fragColor;
	
	// Gamma correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 

	color = vec3(.8, 0.3, .5);//material.albedo; 
	//color = inPos;
	//if(material.albedoIndex<0)
	//else
	//	color = texture(textures[m.albedoIndex], inTexCoord);

	// Ouput
    outColor = vec4(color,1);
}
