#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

#include "../../uniformBufferObject.glsl"
#include "../../material.glsl"
#include "../../light.glsl"
#include "base.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject ubo; };
layout(binding = 1) readonly buffer MaterialArray { Material[] materials; };
layout(binding = 2) uniform sampler2D[] textures; 
layout(binding = 3) readonly buffer LightArray { Light[] lights; };

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in flat int inMaterialIndex;
layout(location = 4) in vec3 inViewPos;

layout(location = 0) out vec4 outColor;

#include "materials/material.glsl"
#include "lights/light.glsl"

void main() 
{
	Material material = materials[inMaterialIndex];
	vec3 n = normalize(inNormal);

	// Direct lighting for analytical lights
	vec3 Ld = vec3(0,0,0);
	vec3 wo = normalize(inViewPos-inPos);// Outgoing direction

	for(int i=0; i<ubo.nLights; i++)
	{
		if(lights[i].type==LIGHT_TYPE_INFINITE)
		{
			Ld += Material_IBL(material, n, wo, lights[i]);
		}
		else
		{
			vec3 wi;// Incident direction
			vec3 Li = Light_sampleLi(lights[i], wi);
			vec3 f = Material_f(material, wi, wo);
			Ld += f*Li*max(0, dot(wi, n));// *1/pdf
		}
	}

	//----- Output -----//
	vec3 color = Ld;
	// HDR tonemapping
    //color = color / (color + vec3(1.0));
	// Gamma correction
    color = pow(color, vec3(1.0/2.2));
    outColor = vec4(color,1);
}
