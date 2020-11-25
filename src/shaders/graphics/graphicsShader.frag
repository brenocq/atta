#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#include "../rayTracing/material.glsl"

layout(binding = 1) readonly buffer MaterialArray { Material[] materials; };
layout(binding = 2) uniform sampler2D[] textureSamplers;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in flat int fragMaterialIndex;
layout(location = 4) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() 
{

	int textureId = -1;
	if(fragMaterialIndex >= 0)
		textureId = materials[fragMaterialIndex].diffuseTextureId;
	const vec3 lightPos = vec3(0, 100, 0);
	const vec3 lightDir = normalize(lightPos - fragPos);
	const float diff = max(dot(normalize(fragNormal), lightDir), 0.5);
	
	vec3 c = fragColor * diff;
	if(textureId >= 0)
	{
		c *= texture(textureSamplers[textureId], fragTexCoord).rgb;
	}

    outColor = vec4(c,1);
}
