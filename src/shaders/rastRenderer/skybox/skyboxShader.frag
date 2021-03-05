#version 460
//#extension GL_ARB_separate_shader_objects : enable
//#extension GL_EXT_nonuniform_qualifier : require
//#extension GL_GOOGLE_include_directive : require
//#include "../material.glsl"
//
////layout (binding = 1) uniform sampler2D equirectangularEnvMap;
//
//layout(location = 0) in vec3 FragColor;
//layout(location = 1) in vec3 FragPos;
//
//layout(location = 0) out vec4 OutColor;
//
//const vec2 invAtan = vec2(0.15915, 0.3183);
//vec2 sampleSphericalMap(vec3 v)
//{
//    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
//    uv *= invAtan;
//    uv += 0.5;
//    return uv;
//}
//
//
void main() 
{
//	vec2 uv = sampleSphericalMap(normalize(FragPos)); // make sure to normalize localPos
//    vec3 color = vec3(0,0,1);//texture(equirectangularEnvMap, uv).rgb;
//
//	float exposure = 3.0f;
//	OutColor.rgb = vec3(1.0) - exp(-color * exposure);
//	OutColor.a = 1.0;
}
