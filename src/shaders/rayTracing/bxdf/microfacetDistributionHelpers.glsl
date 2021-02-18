//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// microfacetDistributionHelpers.glsl
// Date: 2021-02-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_MICROFACET_DISTRIBUTION_HELPERS_GLSL
#define BXDF_MICROFACET_DISTRIBUTION_HELPERS_GLSL
#include "base.glsl"

//-------------------- Beckmann --------------------//
vec3 beckmannSample(vec3 wi, float alpha_x, float alpha_y, float U1, float U2)
{
	// TODO Finish implementation
	// 1. stretch wi
	//Vector3f wiStretched =
	//	Normalize(Vector3f(alpha_x * wi.x, alpha_y * wi.y, wi.z));

	//// 2. simulate P22_{wi}(x_slope, y_slope, 1, 1)
	//Float slope_x, slope_y;
	//BeckmannSample11(CosTheta(wiStretched), U1, U2, &slope_x, &slope_y);

	//// 3. rotate
	//Float tmp = CosPhi(wiStretched) * slope_x - SinPhi(wiStretched) * slope_y;
	//slope_y = SinPhi(wiStretched) * slope_x + CosPhi(wiStretched) * slope_y;
	//slope_x = tmp;

	//// 4. unstretch
	//slope_x = alpha_x * slope_x;
	//slope_y = alpha_y * slope_y;

	// 5. compute normal
	return vec3(0,0,0);//Normalize(Vector3f(-slope_x, -slope_y, 1.f));
}

//-------------------- Trowbridge-Reitz --------------------//
void trowbridgeReitzSample11(float cosTh, float U1, float U2, out float slope_x, out float slope_y)
{
    // special case (normal incidence)
    if(cosTh > .9999)
	{
        float r = sqrt(U1 / (1 - U1));
        float phi = 6.28318530718 * U2;
        slope_x = r * cos(phi);
        slope_y = r * sin(phi);
        return;
    }

    float sinTh = sqrt(max(0.0f, 1.0f - cosTh * cosTh));
    float tanTh = sinTh / cosTh;
    float a = 1 / tanTh;
    float G1 = 2 / (1 + sqrt(1.f + 1.f / (a * a)));

    // sample slope_x
    float A = 2 * U1 / G1 - 1;
    float tmp = 1.f / (A * A - 1.f);
    if(tmp > 1e10) tmp = 1e10;
    float B = tanTh;
    float D = sqrt(max(float(B * B * tmp * tmp - (A * A - B * B) * tmp), 0.0f));
    float slope_x_1 = B * tmp - D;
    float slope_x_2 = B * tmp + D;
    slope_x = (A < 0 || slope_x_2 > 1.f / tanTh) ? slope_x_1 : slope_x_2;

    // sample slope_y
    float S;
    if(U2 > 0.5f)
	{
        S = 1.f;
        U2 = 2.f * (U2 - 0.5f);
    }
	else
	{
        S = -1.f;
        U2 = 2.f * (0.5f - U2);
    }
    float z =
        (U2 * (U2 * (U2 * 0.27385f - 0.73369f) + 0.46341f)) /
        (U2 * (U2 * (U2 * 0.093073f + 0.309420f) - 1.000000f) + 0.597999f);
    slope_y = S * z * sqrt(1.f + slope_x * slope_x);

    //CHECK(!std::isinf(*slope_y));
    //CHECK(!std::isnan(*slope_y));
}

vec3 trowbridgeReitzSample(vec3 wi, float alpha_x, float alpha_y, float U1, float U2)
{
	// 1. stretch wi
	vec3 wiStretched = normalize(vec3(alpha_x * wi.x, alpha_y * wi.y, wi.z));

	// 2. simulate P22_{wi}(x_slope, y_slope, 1, 1)
	float slope_x, slope_y;
	trowbridgeReitzSample11(cosTheta(wiStretched), U1, U2, slope_x, slope_y);

	// 3. rotate
	float tmp = cosPhi(wiStretched) * slope_x - sinPhi(wiStretched) * slope_y;
	slope_y = sinPhi(wiStretched) * slope_x + cosPhi(wiStretched) * slope_y;
	slope_x = tmp;

	// 4. unstretch
	slope_x = alpha_x * slope_x;
	slope_y = alpha_y * slope_y;

	// 5. compute normal
	return normalize(vec3(-slope_x, -slope_y, 1.));
}


#endif// BXDF_MICROFACET_DISTRIBUTION_HELPERS_GLSL
