#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#define PI 3.1415926538
#include "../rayTracing/material.glsl"

layout(binding = 1) readonly buffer MaterialArray { Material[] materials; };
layout(binding = 2) uniform sampler2D[] textureSamplers;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in flat int fragMaterialIndex;

layout(location = 4) in vec3 fragPos;
layout(location = 5) in vec3 viewPos;

layout(location = 0) out vec4 outColor;

const vec3 ambientColor = vec3(.1,.1,.1);
const vec3 lightColor = vec3(.8,.8,.8);
const vec3 lightPos = vec3(0, 10, 0);
const vec3 specularColor = vec3(.3,.3,.3);
const float specularAlpha = 200.0;

void main() 
{
	int textureId = -1;
	if(fragMaterialIndex >= 0)
		textureId = materials[fragMaterialIndex].diffuseTextureId;

	const vec3 l = normalize(lightPos - fragPos);
	const vec3 n = normalize(fragNormal);
	const vec3 v = normalize(viewPos - fragPos);
	const vec3 h = normalize(l+v);
	vec3 S = specularColor*pow(clamp(dot(n, h), 0.0, 1.0), specularAlpha);
	if(dot(n, lightPos)<=0)
		S = S*0;
	const vec3 D = fragColor*clamp(dot(n,  l), 0.0, 1.0);
	const vec3 directColor = lightColor*(D+S);
	
	vec3 color = (ambientColor+directColor);
	if(textureId >= 0)
	{
		color *= texture(textureSamplers[textureId], fragTexCoord).rgb;
	}

    outColor = vec4(color,1);
}
