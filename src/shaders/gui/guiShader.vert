#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "guiStructs.glsl"

layout(binding = 0) readonly uniform GuiUniformBufferObjectStruct { GuiUniformBufferObject uniformBuffer; };
layout(push_constant) uniform GuiObjectInfoStruct { GuiObjectInfo objectInfo; };

layout(location = 0) out vec4 outFragColor;
layout(location = 1) out vec2 outFragPos;

vec2 positions[6] = vec2[](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(0.0, 1.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0)
);

void main() 
{
	vec2 rectCorner = objectInfo.position+positions[gl_VertexIndex]*objectInfo.size;
    gl_Position = vec4(rectCorner,0,1);

    outFragColor = objectInfo.color;
    outFragPos = rectCorner;
}
