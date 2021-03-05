#version 460
//#extension GL_ARB_separate_shader_objects : enable
//#extension GL_GOOGLE_include_directive : require
//#include "../material.glsl"
//#include "../uniformBufferObject.glsl"
//
//layout(binding = 0) readonly uniform UniformBufferObjectStruct 
//{ 
//	UniformBufferObject camera;
//};
//
//layout(binding = 1) readonly buffer MaterialArray {
//	Material[] materials; 
//};
//
//layout(push_constant) uniform ObjectInfo {
//  mat4 modelMat;
//} objectInfo;
//
//layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec3 inNormal;
//layout(location = 2) in vec2 inTexCoord;
//layout(location = 3) in int inMaterialIndex;
//
//out gl_PerVertex
//{
//	vec4 gl_Position;
//};
//
void main() 
{
//	vec3 fragPos = vec3(objectInfo.modelMat * vec4(inPosition, 1.0));
//	vec3 fragNormal = normalize(vec3(transpose(inverse(objectInfo.modelMat)) * vec4(inNormal, 1.0)));
//
//	// Extrude along normal
//	gl_Position = camera.projMat * camera.viewMat * vec4(fragPos,1.0);
}
