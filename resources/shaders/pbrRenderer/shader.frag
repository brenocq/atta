#version 450 core
//---------- Definitions ----------//
struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    int hasNormalTexture;
};
struct PointLight
{
    vec3 position;
    vec3 intensity;
};
struct DirectionalLight
{
    vec3 direction;
    vec3 intensity;
};
const float PI = 3.14159265359;

//---------- Variables ----------//
//----- In/Out -----//
in vec3 worldPos;
in vec3 normal;
in vec2 texCoord;
out vec4 FragColor;

//----- Lighing -----//
#define MAX_NUM_POINT_LIGHTS 10
uniform PointLight pointLights[MAX_NUM_POINT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform int numPointLights;

//----- PBR -----//
uniform vec3 camPos;
// IBL
layout(binding = 0) uniform samplerCube irradianceMap;
layout(binding = 1) uniform samplerCube prefilterMap;
layout(binding = 2) uniform sampler2D   brdfLUT;

//----- Material -----//
uniform Material material;
layout(binding = 3) uniform sampler2D albedoTexture;
layout(binding = 4) uniform sampler2D metallicTexture;
layout(binding = 5) uniform sampler2D roughnessTexture;
layout(binding = 6) uniform sampler2D aoTexture;
layout(binding = 7) uniform sampler2D normalTexture;

//---------- Definitions ----------//
float DistributionGGX(vec3 N, vec3 H, float roughness);// Distribution of microsurface normals
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);// Microsurface shadowing
vec3 fresnelSchlick(float cosTheta, vec3 F0);// Reflect-refract ratio
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

//---------- Main ----------//
void main()
{
    vec3 albedo = material.albedo;
    if(albedo.x == -1.0f)
        albedo = pow(texture(albedoTexture, texCoord).rgb, vec3(2.2));

    float metallic = material.metallic;
    if(metallic == -1.0f)
        metallic = texture(metallicTexture, texCoord).r;

    float roughness = material.roughness;
    if(roughness == -1.0f)
        roughness = texture(roughnessTexture, texCoord).r;

    float ao = material.ao;
    if(ao == -1.0f)
        ao = texture(aoTexture, texCoord).r;

    vec3 N = normalize(normal); 
    vec3 V = normalize(camPos - worldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0      = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    //----- Point lights -----//
    for(int i = 0; i < numPointLights; ++i)
    {
        // Light radiance
        vec3 L = normalize(pointLights[i].position - worldPos);
        vec3 H = normalize(V + L);
        float distance    = length(pointLights[i].position - worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = pointLights[i].intensity * attenuation;

        // Cook-torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);       
        float G   = GeometrySmith(N, V, L, roughness); 
        vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
        vec3 specular     = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        // Add outgoing radiance to Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // diffuse IBL
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    // Specular IBL
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);	

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color   = ambient + Lo;

    // HDR
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0f);
}

//---------- Declarations ----------//
float DistributionGGX(vec3 N, vec3 H, float roughness)
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

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
