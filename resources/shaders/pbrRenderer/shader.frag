#version 300 es
precision mediump float;
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
in vec4 posDirectionalLightSpace;
in vec3 normal;
in vec2 texCoord;
out vec4 FragColor;

//----- Lighing -----//
#define MAX_NUM_POINT_LIGHTS 10
uniform PointLight pointLights[MAX_NUM_POINT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform sampler2D directionalShadowMap;
uniform mat3 environmentLightOri;
uniform samplerCube omniShadowMap;
uniform float omniFarPlane;

uniform int numPointLights;
uniform int numDirectionalLights;
uniform int numEnvironmentLights;

//----- PBR -----//
uniform vec3 camPos;
// IBL
uniform samplerCube prefilterMap;
uniform samplerCube irradianceMap;
uniform sampler2D brdfLUT;

//----- Material -----//
uniform Material material;
uniform sampler2D albedoTexture;
uniform sampler2D metallicTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D aoTexture;
uniform sampler2D normalTexture;

//---------- Definitions ----------//
vec3 calcLightContribution(vec3 L, vec3 N, vec3 V, vec3 radiance, vec3 albedo, float metallic, float roughness, vec3 F0);
float directionalShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float omniShadowCalculation(vec3 lightPos, vec3 normal);

float distributionGGX(vec3 N, vec3 H, float roughness);// Distribution of microsurface normals
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);// Microsurface shadowing
vec3 fresnelSchlick(float cosTheta, vec3 F0);// Reflect-refract ratio
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

//---------- Main ----------//
void main()
{
    vec3 albedo = material.albedo;
    if(albedo.x < 0.0f)
        albedo = pow(texture(albedoTexture, texCoord).rgb, vec3(2.2));

    float metallic = material.metallic;
    if(metallic < 0.0f)
        metallic = texture(metallicTexture, texCoord).r;

    float roughness = material.roughness;
    if(roughness < 0.0f)
        roughness = texture(roughnessTexture, texCoord).r;

    float ao = material.ao;
    if(ao < 0.0f)
        ao = texture(aoTexture, texCoord).r;

    vec3 N = normal; 
    vec3 V = normalize(camPos - worldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    //----- Point lights -----//
    for(int i = 0; i < numPointLights; i++)
    {
        vec3 L = normalize(pointLights[i].position - worldPos);
        vec3 H = normalize(V + L);
        float dist = length(pointLights[i].position - worldPos);
        float attenuation = 1.0/(dist * dist);
        vec3 radiance = pointLights[i].intensity * attenuation;

        float shadow = 0.0f;
        shadow = omniShadowCalculation(pointLights[i].position, N);
        Lo += calcLightContribution(L, N, V, radiance, albedo, metallic, roughness, F0) * (1.0f - shadow);
    }

    //----- Directional light -----//
    if(numDirectionalLights == 1)
    {
        vec3 L = -directionalLight.direction;
        vec3 radiance = directionalLight.intensity;

        float shadow = directionalShadowCalculation(posDirectionalLightSpace, N, L);
        Lo += calcLightContribution(L, N, V, radiance, albedo, metallic, roughness, F0) * (1.0f - shadow);
    }

    //----- Environment light -----//
    vec3 ambient = vec3(0.0f);
    if(numEnvironmentLights == 1)
    {
        // Diffuse IBL
        vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
        vec3 kS = F;
        vec3 kD = 1.0 - kS;
        kD *= 1.0 - metallic;
        vec3 irradiance = texture(irradianceMap, environmentLightOri*N).rgb;
        vec3 diffuse = irradiance * albedo;

        // Specular IBL
        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureLod(prefilterMap, environmentLightOri*R, roughness * MAX_REFLECTION_LOD).rgb;    
        vec2 brdf = texture(brdfLUT, vec2(max(dot(environmentLightOri*N, V), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);	

        ambient = (kD*diffuse + specular) * ao;
    }
    else if(numEnvironmentLights == 0)
    {
        ambient = vec3(0.3) * albedo * ao;
    }

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color/(color + vec3(1.0));
    // Gamma correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0f);
}

vec3 calcLightContribution(vec3 L, vec3 N, vec3 V, vec3 radiance, vec3 albedo, float metallic, float roughness, vec3 F0)
{
    vec3 H = normalize(V + L);

    // Cook-torrance BRDF
    float D = distributionGGX(N, H, roughness);       
    float G = geometrySmith(N, V, L, roughness); 
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = D * G * F;
    float denominator = 4.0f * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
    vec3 specular = numerator/denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    // Add outgoing radiance to Lo
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

float directionalShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(directionalShadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.0001);
    float shadow = 0.0;
    ivec2 texSize = textureSize(directionalShadowMap, 0);
    vec2 texelSize = 1.0f/vec2(texSize);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 25.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

float omniShadowCalculation(vec3 lightPos, vec3 normal)
{
    // get vector between fragment position and light position
    vec3 fragToLight = worldPos - lightPos;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(omniShadowMap, fragToLight).r;
    if(closestDepth == 1.0f) return 0.0f;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= omniFarPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float shadow = 0.0;
    float bias   = 0.15;
    int samples  = 20;
    float viewDistance = length(camPos - worldPos);
    float diskRadius = (1.0 + (viewDistance / omniFarPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(omniShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= omniFarPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);  

    return shadow;
}

//---------- Declarations ----------//
float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num/denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num/denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometrySchlickGGX(NdotV, roughness);
    float ggx1  = geometrySchlickGGX(NdotL, roughness);

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
