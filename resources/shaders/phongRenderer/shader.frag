#version 300 es
precision mediump float;
//---------- Definitions ----------//
struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
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

//---------- Variables ----------//
//----- In/Out -----//
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;
out vec4 FragColor;

//----- Phong -----//
uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

//----- Lighing -----//
#define MAX_NUM_POINT_LIGHTS 10
uniform PointLight pointLights[MAX_NUM_POINT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform int numPointLights;

//----- Material -----//
uniform Material material;
uniform sampler2D albedoTexture;
uniform sampler2D metallicTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D aoTexture;

//---------- Definitions ----------//
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//---------- Main ----------//
vec3 albedo;
float metallic;
float roughness;
float ao;
void main()
{
    // Initialize material
    albedo = material.albedo;
    if(albedo.x < 0.0f)
        //albedo = texture(albedoTexture, texCoord).rgb;
        albedo = vec3(texCoord.x, texCoord.y, 1.0f);

    metallic = material.metallic;
    if(metallic < 0.0f)
        metallic = texture(metallicTexture, texCoord).r;

    roughness = material.roughness;
    if(roughness < 0.0f)
        roughness = texture(roughnessTexture, texCoord).r;

    ao = material.ao;
    if(ao < 0.0f)
        ao = texture(aoTexture, texCoord).r;

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirectionalLight(directionalLight, norm, viewDir);
    for(int i = 0; i < numPointLights; i++)
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);  	

    result += ambientColor * ambientStrength * albedo;
    result *= ao;

    FragColor = vec4(result, 1.0f);
}

//---------- Declarations ----------//
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.0f);
    // combine results
    float diffuse  = diffuseStrength * diff * roughness;
    float specular = specularStrength * spec * metallic;
    return light.intensity * (diffuse+specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5.0f);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (2.0f + 0.4f*distance + 0.64f*(distance*distance));    
    // combine results
    float diffuse  = diffuseStrength * diff * roughness;
    float specular = specularStrength * spec * metallic;
    return light.intensity * attenuation * (diffuse + specular);
}
