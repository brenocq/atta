#version 450 core
struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

in vec3 fragPos;
in vec3 normal;

uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform Material material;

out vec4 FragColor;

void main()
{
	//----- Ambient -----//
	vec3 ambient = ambientStrength * ambientColor * material.ao;

	//----- Diffuse -----//
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =  diffuseStrength * diff * lightColor * material.roughness;

	//----- Specular -----//
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor * material.metallic;

	vec3 result = (ambient + diffuse + specular) * material.albedo;
	FragColor = vec4(result, 1.0f);
}
