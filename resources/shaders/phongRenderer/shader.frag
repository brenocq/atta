#version 450 core
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

//---------- Definitions ----------//
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//---------- Main ----------//
void main()
{
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = calcDirectionalLight(directionalLight, norm, viewDir);
    for(int i = 0; i < numPointLights; i++)
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);  	

	result += ambientColor * ambientStrength * material.albedo;
	result *= material.ao;

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
    float diffuse  = diffuseStrength * diff * material.roughness;
    float specular = specularStrength * spec * material.metallic;
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
    float diffuse  = diffuseStrength * diff * material.roughness;
    float specular = specularStrength * spec * material.metallic;
    return light.intensity * attenuation * (diffuse + specular);
}
