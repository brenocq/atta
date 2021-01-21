#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "guiStructs.glsl"

layout(binding = 0) readonly uniform GuiUniformBufferObjectStruct { GuiUniformBufferObject ubo; };
layout(binding = 1) uniform sampler2D[] textureSamplers;
layout(push_constant) uniform GuiObjectInfoStruct { GuiObjectInfo objectInfo; };

layout(location = 0) in vec4 inFragColor;
layout(location = 1) in vec2 inFragPos;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

void main() 
{
	if(objectInfo.isLetter == 0)
	{
		// Is box
		vec2 objPos = objectInfo.position*2-vec2(ubo.ratio,1.0);// [0,1][0,1] -> [-1,1][-1,1]
		vec2 objSize = objectInfo.size*2;// New coord size
		vec2 fragPosRatio = vec2(inFragPos.x*ubo.ratio, inFragPos.y);
		vec2 objectPosRatio = vec2(objPos.x*ubo.ratio, objPos.y);
		vec2 objectSizeRatio = vec2(objSize.x*ubo.ratio, objSize.y);
		vec2 topLeft = objectPosRatio    + vec2(objectInfo.radius*ubo.ratio, objectInfo.radius);
		vec2 topRight = objectPosRatio;//   + vec2(ubo.ratio*objectSizeRatio/2 - objectInfo.radius*ubo.ratio, objectInfo.radius);
		vec2 bottomLeft = objectPosRatio + vec2(0,1)*objectSizeRatio		   + vec2(objectInfo.radius*ubo.ratio, -objectInfo.radius);
		vec2 bottomRight = objectPosRatio+ vec2(1*ubo.ratio,1)*objectSizeRatio + vec2(-objectInfo.radius*ubo.ratio, -objectInfo.radius);

		// Check distance to create round borders
		if(inFragPos.x > topRight.x && inFragPos.y < topRight.y)
		{
			bool inside = length(inFragPos-topRight)<objectInfo.radius;
			inside = false;
			outColor = vec4(inFragColor.xyz, inside?inFragColor.w:0.0f);
		}
		else if(inFragPos.x > bottomRight.x && inFragPos.y > bottomRight.y)
		{
			bool inside = length(inFragPos-bottomRight)<objectInfo.radius;
			inside = true;
			outColor = vec4(inFragColor.xyz, inside?inFragColor.w:0.0f);
		}
		else if(inFragPos.x < bottomLeft.x && inFragPos.y > bottomLeft.y)
		{
			bool inside = length(inFragPos-bottomLeft)<objectInfo.radius;
			inside = true;
			outColor = vec4(inFragColor.xyz, inside?inFragColor.w:0.0f);
		}
		else if(inFragPos.x < topLeft.x && inFragPos.y < topLeft.y)
		{
			bool inside = length(inFragPos-topLeft)<objectInfo.radius;
			inside = true;
			outColor = vec4(inFragColor.xyz, inside?inFragColor.w:0.0f);
		}
		else
			outColor = inFragColor;
	}
	else
	{
		// Is letter
		outColor = objectInfo.color*texture(textureSamplers[0], inTexCoord).rgba;
	}
}
