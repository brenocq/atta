#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../../uniformBufferObject.glsl"
#include "point.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct { UniformBufferObject camera; };
layout(binding = 1) readonly buffer PointsArray { Point[] pointsToDraw; };

layout(location = 0) out vec3 outFragColor;

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
};

void main() 
{
	vec3 point = pointsToDraw[gl_VertexIndex].p.xyz;
	vec3 color = pointsToDraw[gl_VertexIndex].c.xyz;

	mat4 projection = camera.projMat;
	mat4 view = camera.viewMat;
	const vec4 fragPos = (projection * view) * vec4(point, 1.0);

    gl_Position = fragPos;
    //gl_Position = vec4(0,0,.5,1);
    outFragColor = color;
	gl_PointSize = 2.0f;
}
