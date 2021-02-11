#version 450
#extension GL_GOOGLE_include_directive : require
#include "../material.glsl"

layout(binding = 1) readonly buffer MaterialArray
{
	Material[] materials; 
};

layout(location = 0) in flat int inMaterialIndex;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	outFragColor = vec4(materials[inMaterialIndex].albedo, 1.0); 
}
