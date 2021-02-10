#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require
#include "../uniformBufferObject.glsl"
#include "line.glsl"

layout(binding = 0) readonly uniform UniformBufferObjectStruct 
{ 
	UniformBufferObject camera;
};

layout(binding = 1) readonly buffer LinesArray
{ 
	Line[] linesToDraw;
};

layout(location = 0) out vec3 outFragColor;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	//vec3 point = gl_VertexIndex%2==0 ? linesToDraw[gl_VertexIndex/2].p0 : linesToDraw[gl_VertexIndex/2].p1;
	vec3 point = gl_VertexIndex%2==0 ? vec3(0,-10,0) : vec3(0,10,0);
	//vec3 color = gl_VertexIndex%2==0 ? linesToDraw[gl_VertexIndex/2].c0 : linesToDraw[gl_VertexIndex/2].c1;
	vec3 color = gl_VertexIndex%2==0 ? vec3(1,0,0) : vec3(0,0,1);

	mat4 projection = transpose(camera.projMat);
	mat4 view = transpose(camera.viewMat);
	const vec4 fragPos = (projection * view) * vec4(point, 1.0);

    gl_Position = fragPos;
    outFragColor = color;
}
