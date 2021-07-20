#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../../uniformBufferObject.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject ubo; };
layout(binding = 1) uniform samplerCube samplerCubeMap;

layout(location = 0) in vec3 inUVW;
layout(location = 0) out vec4 outColor;

//const vec2 invAtan = vec2(0.1591, 0.3183);
//vec2 sampleSphericalMap(vec3 v)
//{
//    vec2 uv = vec2(atan(v.z, v.x), -asin(v.y));
//    uv *= invAtan;
//    uv += 0.5;
//	uv.x = 1-uv.x;
//	uv.x += 0.5;
//    return uv;
//}

void main() 
{
	// Calculate vector from camera focues to world
	//vec2 screenPos = inFragPos;
	////screenPos.y*=-1;
	//vec3 viewVector = (ubo.viewMatInverse*ubo.projMatInverse*vec4(screenPos,1,1)).xyz;

	//vec2 uv = sampleSphericalMap(normalize(viewVector)); // make sure to normalize localPos
    //vec3 color = texture(equirectangularEnvMap, uv).rgb;

	vec3 color = texture(samplerCubeMap, inUVW).rgb;
	// Gamma correction
	outColor = vec4(inUVW,1);
}
