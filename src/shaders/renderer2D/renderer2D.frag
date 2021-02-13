#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require
#include "../material.glsl"

layout(binding = 1) readonly buffer MaterialArray
{
	Material[] materials; 
};

layout(binding = 2) uniform sampler2D[] textures;

layout(location = 0) in flat int inMaterialIndex;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	Material m = materials[inMaterialIndex];
	if(m.albedoIndex >= 0)
		outFragColor = texture(textures[m.albedoIndex], inTexCoord);
	else
		outFragColor = vec4(m.albedo, 1.0); 
}
