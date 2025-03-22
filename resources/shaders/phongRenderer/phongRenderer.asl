struct Material {
    vec3 albedo;
    float pad0;
    float metallic;
    float roughness;
    float ao;
};
struct PointLight {
    vec3 position;
    float pad0;
    vec3 intensity;
    float pad1;
};
struct DirectionalLight {
    vec3 direction;
    float pad0;
    vec3 intensity;
    float pad1;
};

perFrame mat4 uView;
perFrame mat4 uProjection;
perFrame vec3 uViewPos;
perFrame float pad0;
perFrame vec3 uAmbientColor;
perFrame float pad1;
perFrame float uAmbientStrength;
perFrame float uDiffuseStrength;
perFrame float uSpecularStrength;
perFrame bool uHasDirectionalLight;
perFrame DirectionalLight uDirectionalLight;

#define MAX_POINT_LIGHTS 10
perFrame int uNumPointLights;
perFrame PointLight uPointLights[MAX_POINT_LIGHTS];

perDraw mat4 uModel;
perDraw mat4 uInvModel;
perDraw Material uMaterial;
perDraw sampler2D uAlbedoTexture;
perDraw sampler2D uMetallicTexture;
perDraw sampler2D uRoughnessTexture;
perDraw sampler2D uAoTexture;

perVertex vec3 vFragPos;
perVertex vec3 vNormal;
perVertex vec2 vUV;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec4 vertex(vec3 iVertex, vec3 iNormal, vec2 iUV) {
    vec4 coord = uModel * vec4(iVertex, 1.0f);
    vFragPos = coord.xyz;
    vNormal = mat3(transpose(uInvModel)) * iNormal;
    vUV = iUV;
    return uProjection * uView * coord;
}

vec3 albedo;
float metallic;
float roughness;
float ao;
float alpha;
void fragment(out vec4 color) {
    // Initialize material
    albedo = uMaterial.albedo;
    alpha = 1.0f;
    if (albedo.x < 0.0f) {
        albedo = texture(uAlbedoTexture, vUV).rgb;
        alpha = texture(uAlbedoTexture, vUV).a;
    }

    if (alpha < 0.1)
        discard;

    metallic = uMaterial.metallic;
    if (metallic < 0.0f)
        metallic = texture(uMetallicTexture, vUV).r;

    roughness = uMaterial.roughness;
    if (roughness < 0.0f)
        roughness = texture(uRoughnessTexture, vUV).r;

    ao = uMaterial.ao;
    if (ao < 0.0f)
        ao = texture(uAoTexture, vUV).r;

    vec3 result = vec3(0.0f);

    // Directional light
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(uViewPos - vFragPos);
    if (uHasDirectionalLight)
        result += calcDirectionalLight(uDirectionalLight, norm, viewDir);

    // Point light
    if (uNumPointLights > 0 && uNumPointLights <= MAX_POINT_LIGHTS)
        for (int i = 0; i < uNumPointLights; i++)
            result += calcPointLight(uPointLights[i], norm, vFragPos, viewDir);

    result += uAmbientColor * uAmbientStrength * albedo;
    result *= ao;

    color = vec4(result, 1.0f);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.0f);
    // combine results
    float diffuse = uDiffuseStrength * diff * roughness;
    float specular = uSpecularStrength * spec * metallic;
    return light.intensity * (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5.0f);
    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (2.0f + 2.0f * dist + 2.0 * (dist * dist));
    // combine results
    float diffuse = uDiffuseStrength * diff * roughness;
    float specular = uSpecularStrength * spec * metallic;
    return light.intensity * attenuation * (diffuse + specular);
}
