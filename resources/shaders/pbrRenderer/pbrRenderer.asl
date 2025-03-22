struct Material {
    float metallic;
    float roughness;
    float ao;
    vec3 albedo;
};
struct PointLight {
    vec3 position;
    vec3 intensity;
};
struct DirectionalLight {
    vec3 direction;
    vec3 intensity;
};
const float PI = 3.14159265359;

//----- Per Frame -----//
perFrame mat4 projection;
perFrame mat4 view;
perFrame mat4 directionalLightMatrix;
perFrame vec3 camPos;
perFrame float p0;// Dummy

perFrame int numPointLights;
perFrame int numDirectionalLights;
perFrame int numEnvironmentLights;

// Point light
#define MAX_NUM_POINT_LIGHTS 10
perFrame PointLight pointLights[MAX_NUM_POINT_LIGHTS];

// Directional light - Shadow map
perFrame DirectionalLight directionalLight;
// perFrame sampler2D directionalShadowMap;
// perFrame samplerCube omniShadowMap;
perFrame float omniFarPlane;

// Environment light - IBL
perFrame mat3 environmentLightOri;
// perFrame samplerCube prefilterMap;
// perFrame samplerCube irradianceMap;
// perFrame sampler2D brdfLUT;

//----- Per Draw -----//
perDraw mat4 model;
perDraw mat4 invModel;
perDraw Material material;
perDraw sampler2D albedoTexture;
perDraw sampler2D metallicTexture;
perDraw sampler2D roughnessTexture;
perDraw sampler2D aoTexture;
perDraw sampler2D normalTexture;

perVertex vec3 worldPos;
perVertex vec4 posDirectionalLightSpace;
perVertex vec3 normal;
perVertex vec2 texCoord;

//----- Vertex Shader -----//
vec4 vertex(vec3 inPosition, vec3 inNormal, vec2 inTexCoord) {
    vec4 coord = model * vec4(inPosition, 1.0f);
    vec4 coordImage = view * coord;

    //  float z = coordImage.z;
    //  coordImage /= z;
    //  float r = sqrt(coordImage.x*coordImage.x + coordImage.y*coordImage.y);
    //  float L = 1 + r*0.103689 + r*r*0.00487908 + r*r*r*0.00116894 + r*r*r*r*0.000941614;
    //  coordImage.x *= L;
    //  coordImage.y *= L;
    //  coordImage *= z;

    worldPos = coord.xyz;
    posDirectionalLightSpace = directionalLightMatrix * vec4(worldPos, 1.0f);
    normal = mat3(transpose(invModel)) * inNormal;
    texCoord = inTexCoord;

    return projection * coordImage;
}

//----- Fragment Shader -----//
vec3 calcLightContribution(vec3 L, vec3 N, vec3 V, vec3 radiance, vec3 albedo, float metallic, float roughness, vec3 F0);
float directionalShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float omniShadowCalculation(vec3 lightPos, vec3 normal);
float distributionGGX(vec3 N, vec3 H, float roughness);       // Distribution of microsurface normals
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness); // Microsurface shadowing
vec3 fresnelSchlick(float cosTheta, vec3 F0);                 // Reflect-refract ratio
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

void fragment(out vec4 outColor) {
    // Calculate material parameters
    vec3 albedo = material.albedo;
    if (albedo.x < 0.0f)
        albedo = pow(texture(albedoTexture, texCoord).rgb, vec3(2.2));

    float metallic = material.metallic;
    if (metallic < 0.0f)
        metallic = texture(metallicTexture, texCoord).r;

    float roughness = material.roughness;
    if (roughness < 0.0f)
        roughness = texture(roughnessTexture, texCoord).r;

    float ao = material.ao;
    if (ao < 0.0f)
        ao = texture(aoTexture, texCoord).r;

    // Calculate vectors
    vec3 N = normalize(normal);
    vec3 V = normalize(camPos - worldPos);

    // Fresnel coefficient
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    //----- Point lights -----//
    for (int i = 0; i < numPointLights; i++) {
        // Compute light direction
        vec3 L = normalize(pointLights[i].position - worldPos);

        // Compute radiance
        float dist = length(pointLights[i].position - worldPos);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = pointLights[i].intensity * attenuation;

        float shadow = 0.0f;
        // shadow = omniShadowCalculation(pointLights[i].position, N);
        Lo += calcLightContribution(L, N, V, radiance, albedo, metallic, roughness, F0) * (1.0f - shadow);
    }

    //----- Directional light -----//
    if (numDirectionalLights == 1) {
        vec3 L = -directionalLight.direction;
        vec3 radiance = directionalLight.intensity;

        float shadow = 0.0f; // directionalShadowCalculation(posDirectionalLightSpace, N, L);
        Lo += calcLightContribution(L, N, V, radiance, albedo, metallic, roughness, F0) * (1.0f - shadow);
    }

    //----- Environment light -----//
    vec3 ambient = vec3(0.0f);
    if (numEnvironmentLights == 1) {
        // Diffuse IBL
        vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
        vec3 kS = F;
        vec3 kD = 1.0 - kS;
        kD *= 1.0 - metallic;
        vec3 irradiance = vec3(1, 1, 1); // texture(irradianceMap, environmentLightOri * N).rgb;
        vec3 diffuse = irradiance * albedo;

        // Specular IBL
        const float MAX_REFLECTION_LOD = 4.0;
        vec3 R = reflect(-V, N);
        vec3 prefilteredColor = vec3(1, 1, 1); // textureLod(prefilterMap, environmentLightOri * R, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf = vec2(0, 0);                // texture(brdfLUT, vec2(max(dot(environmentLightOri * N, V), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

        ambient = (kD * diffuse + specular) * ao;
    } else if (numEnvironmentLights == 0) {
        ambient = vec3(0.03) * albedo * ao;
    }

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    outColor = vec4(color, 1.0f);
}

vec3 calcLightContribution(vec3 L, vec3 N, vec3 V, vec3 radiance, vec3 albedo, float metallic, float roughness, vec3 F0) {
    vec3 H = normalize(V + L);

    // Cook-torrance BRDF
    float D = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 num = D * G * F;
    float denom = 4.0f * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = num / denom;

    vec3 kS = F;              // Reflection/specular fraction
    vec3 kD = vec3(1.0) - kS; // Refraction/diffuse fraction
    kD *= 1.0 - metallic;

    // Add outgoing radiance to Lo
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

float directionalShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = 0.0f; // texture(directionalShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.0001);
    float shadow = 0.0;
    ivec2 texSize = ivec2(0, 0); // textureSize(directionalShadowMap, 0);
    vec2 texelSize = 1.0f / vec2(texSize);
    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            float pcfDepth = 0.0f; // texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 25.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 sampleOffsetDirections[20] =
    vec3[](vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
           vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
           vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

float omniShadowCalculation(vec3 lightPos, vec3 normal) {
    // get vector between fragment position and light position
    vec3 fragToLight = worldPos - lightPos;
    // use the light to fragment vector to sample from the depth map
    float closestDepth = 0.0f; // texture(omniShadowMap, fragToLight).r;
    if (closestDepth == 1.0f)
        return 0.0f;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= omniFarPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(camPos - worldPos);
    float diskRadius = (1.0 + (viewDistance / omniFarPlane)) / 25.0;
    for (int i = 0; i < samples; ++i) {
        float closestDepth = 0.0f;    // texture(omniShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= omniFarPlane; // undo mapping [0;1]
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    // Trowbridge-Reitz GGX normal distribution function
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    // Compute k for direct lighting
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    // Smith's method to take into account both view direction and light direction
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) { return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0); }

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
