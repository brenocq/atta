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
		// Convert coordinates to [ratio, 1]
		vec2 fragPos = ((inFragPos+vec2(1,1))/2.0)*vec2(ubo.ratio,1.0);
		
		vec2 objPos = objectInfo.position*vec2(ubo.ratio,1.0);// [0,1][0,1] -> [0,ratio][0,1]
		vec2 objHalfSize = objectInfo.size*vec2(ubo.ratio,1.0)*0.5;// New coord size
		vec2 objPosCenter = objPos+objHalfSize;

		// Radius centers
		vec2 topLeft 	 = 	objPosCenter + vec2(-objHalfSize.x, -objHalfSize.y)	+ vec2(objectInfo.radius,objectInfo.radius);
		vec2 topRight 	 = 	objPosCenter + vec2(objHalfSize.x, -objHalfSize.y)	+ vec2(-objectInfo.radius,objectInfo.radius);
		vec2 bottomLeft  = 	objPosCenter + vec2(-objHalfSize.x, objHalfSize.y)	+ vec2(objectInfo.radius,-objectInfo.radius);
		vec2 bottomRight = 	objPosCenter + vec2(objHalfSize.x, objHalfSize.y)	+ vec2(-objectInfo.radius,-objectInfo.radius);

		// Check distance to create round borders
		if(fragPos.x > topRight.x && fragPos.y < topRight.y)
		{
			bool inside = length(fragPos-topRight)<objectInfo.radius;
			outColor = vec4(inFragColor.xyz, inside?inFragColor.w:0.0f);
		}
		else if(fragPos.x > bottomRight.x && fragPos.y > bottomRight.y)
		{
			bool inside = length(fragPos-bottomRight)<objectInfo.radius;
			outColor = vec4(inFragColor.xyz, inside?inFragColor.w:0.0f);
		}
		else if(fragPos.x < bottomLeft.x && fragPos.y > bottomLeft.y)
		{
			bool inside = length(fragPos-bottomLeft)<objectInfo.radius;
			outColor = vec4(inFragColor.xyz, inside?inFragColor.w:0.0f);
		}
		else if(fragPos.x < topLeft.x && fragPos.y < topLeft.y)
		{
			bool inside = length(fragPos-topLeft)<objectInfo.radius;
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
