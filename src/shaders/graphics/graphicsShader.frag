#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#include "../rayTracing/material.glsl"

layout(binding = 1) readonly buffer MaterialArray { Material[] Materials; };
layout(binding = 2) uniform sampler2D[] TextureSamplers;

layout(location = 0) in vec3 FragColor;
layout(location = 1) in vec3 FragNormal;
layout(location = 2) in vec2 FragTexCoord;
layout(location = 3) in flat int FragMaterialIndex;
layout(location = 4) in vec3 FragPos;

layout(location = 0) out vec4 OutColor;

void main() 
{
	const int textureId = Materials[FragMaterialIndex].diffuseTextureId;
	const vec3 lightPos = vec3(0, 100, 0);
	const vec3 lightDir = normalize(lightPos - FragPos);
	const float diff = max(dot(normalize(FragNormal), lightDir), 0.2);
	
	vec3 c = FragColor * diff;
	if(textureId >= 0)
	{
		c *= texture(TextureSamplers[textureId], FragTexCoord).rgb;
	}

    OutColor = vec4(c,1);
}
