#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require

//layout(binding = 1) readonly buffer MaterialArray { Material[] Materials; };
//layout(binding = 2) uniform sampler2D[] TextureSamplers;

layout(location = 0) in vec3 inFragColor;
layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = vec4(inFragColor,1);
}
