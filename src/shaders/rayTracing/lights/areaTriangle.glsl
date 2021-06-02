//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// areaTriangle.glsl
// Date: 2021-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_AREA_TRIANGLE_GLSL
#define LIGHTS_AREA_TRIANGLE_GLSL
#include "base.glsl"

void AreaTriangleLight_sample(Light light, vec2 u, out vec3 pWorld, out vec3 n)
{
	vec3 p0 = light.datav[1].xyz;
	vec3 p1 = light.datav[2].xyz;
	vec3 p2 = light.datav[3].xyz;
	vec2 b = uniformSampleTriangle(u);
	// Compute triangle normal from vertices
    n = normalize(cross(p1-p0,p2-p0));

    pWorld = b.x*p0 + b.y*p1+(1-b.x-b.y)*p2;
}   
    
float AreaTriangleLight_pdf(Light light)
{   
    // Probability of each point on the surface being chosen
	vec3 p0 = light.datav[1].xyz;
	vec3 p1 = light.datav[2].xyz;
	vec3 p2 = light.datav[3].xyz;
	float area = 0.5*cross(p1-p0,p2-p0).length();
    return 1/area;
}   

float AreaTriangleLight_pdfLi(Light light, Interaction it, vec3 wi)
{
	//if(Light_occluded(it.point, wi, 1000.0f))
	//	return 0;

	vec3 p0 = light.datav[1].xyz;
	vec3 p1 = light.datav[2].xyz;
	vec3 p2 = light.datav[3].xyz;
	float area = 0.5*cross(p1-p0,p2-p0).length();
	vec3 n = normalize(cross(p1-p0, p2-p0));

	// Point triangle intersection
	vec3 pTriInt = p0;

	float lengthSquared = dot(p0-pTriInt,p0-pTriInt);
	float pdf = lengthSquared/abs(dot(n, -wi))*area;
	if(isinf(pdf)) return 0;
	return pdf;
}
    
//float AreaTriangleLight_pdf(Light light, vec3 surfaceP, vec3 lightP, vec3 lightN, vec3 wi)
//{ 
//    // Probability of each point on the surface being chosen given direction (with respect to the solid angle)
//    float radius = light.datav[1].x;
//    float height = light.datav[1].y;
//    float phiMax = light.datav[1].z;
//    float area = (phiMax*radius*height);
//
//    float dist = length(surfaceP-lightP); 
//    float distanceSqr = dist*dist;  
//    return distanceSqr/(abs(dot(lightN, -wi))*area);
//}

vec3 AreaTriangleLight_sampleLi(
        Light light, VisibilityPoint ref, vec2 u,
        out vec3 wi, out float pdf, out VisibilityTester vis)
{
    vec3 pTriangle, n;
    AreaTriangleLight_sample(light, u, pTriangle, n);

    wi = normalize(pTriangle - ref.point);
    pdf = AreaTriangleLight_pdf(light);//, ref.point, pTriangle, n, wi);
    vis.p0 = ref;
    vis.p1.point = pTriangle;

	vec3 radiance = light.datav[0].xyz;
    return radiance;
}

vec3 AreaTriangleLight_power(Light light)
{
	vec3 radiance = light.datav[0].xyz;
	vec3 p0 = light.datav[1].xyz;
	vec3 p1 = light.datav[2].xyz;
	vec3 p2 = light.datav[3].xyz;
	float area = 0.5*cross(p1-p0,p2-p0).length();

    return 3.1415926535f * radiance * area;
}

uint AreaTriangleLight_flags()
{
    return LIGHT_FLAG_AREA;
}

#endif// LIGHTS_AREA_TRIANGLE_GLSL
