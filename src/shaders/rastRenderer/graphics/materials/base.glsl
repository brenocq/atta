//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// base.glsl
// Date: 2021-03-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_MATERIALS_BASE_GLSL
#define ATTA_RAST_MATERIALS_BASE_GLSL

bool isBlack(vec3 v)
{
	return v.x==0 && v.y==0 && v.z==0;
}

float cosTheta(vec3 w) { return w.z; }
float cos2Theta(vec3 w) { return w.z*w.z; }
float absCosTheta(vec3 w) { return abs(w.z); }
float sin2Theta(vec3 w) { return max(0, 1-cos2Theta(w)); }
float sinTheta(vec3 w) { return sqrt(sin2Theta(w)); }
float tanTheta(vec3 w) { return sinTheta(w)/cosTheta(w); }
float tan2Theta(vec3 w) { return sin2Theta(w)/cos2Theta(w); }
float cosPhi(vec3 w)
{ 
	float sinT = sinTheta(w);
	return (sinT==0) ? 1 : clamp(w.x/sinT, -1, 1);
}
float sinPhi(vec3 w)
{ 
	float sinT = sinTheta(w);
	return (sinT==0) ? 0 : clamp(w.y/sinT, -1, 1);
}
float cos2Phi(vec3 w) { return cosPhi(w)*cosPhi(w); }
float sin2Phi(vec3 w) { return sinPhi(w)*sinPhi(w); }


// Disney aux
float sqr(float x) { return x*x; }
float schlickR0FromEta(float eta) { return sqr(eta - 1) / sqr(eta + 1); }

float lum_y(vec3 rgb)
{
	return 0.212671f*rgb.x + 0.715160f*rgb.y + 0.072169f*rgb.z;
}

float SchlickFresnel(float u)
{
    float m = clamp(1.0 - u, 0.0, 1.0);
    float m2 = m * m;
    return m2 * m2 * m; // pow(m,5)
}

float DielectricFresnel(float cos_theta_i, float eta)
{
    float sinThetaTSq = eta * eta * (1.0f - cos_theta_i * cos_theta_i);

    // Total internal reflection
    if (sinThetaTSq > 1.0)
        return 1.0;

    float cos_theta_t = sqrt(max(1.0 - sinThetaTSq, 0.0));

    float rs = (eta * cos_theta_t - cos_theta_i) / (eta * cos_theta_t + cos_theta_i);
    float rp = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);

    return 0.5f * (rs * rs + rp * rp);
}

float GTR2(float NDotH, float a)
{
    float a2 = a * a;
    float t = 1.0 + (a2 - 1.0) * NDotH * NDotH;
    return a2*invPi/(t*t);
}

float GTR2_aniso(float NDotH, float HDotX, float HDotY, float ax, float ay)
{
    float a = HDotX / ax;
    float b = HDotY / ay;
    float c = a * a + b * b + NDotH * NDotH;
    return 1.0*invPi / (ax * ay * c * c);
}

float SmithG_GGX(float NDotV, float alphaG)
{
    float a = alphaG * alphaG;
    float b = NDotV * NDotV;
    return 1.0 / (NDotV + sqrt(a + b - a * b));
}

float SmithG_GGX_aniso(float NDotV, float VDotX, float VDotY, float ax, float ay)
{
    float a = VDotX * ax;
    float b = VDotY * ay;
    float c = NDotV;
    return 1.0 / (NDotV + sqrt(a * a + b * b + c * c));
}

// Unreal Engine 4 aux
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    float m = max(1.0 - cosTheta, 0.0);
    return F0 + (1-F0)*(m*m)*(m*m)*m;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    float m = max(1.0 - cosTheta, 0.0);
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * (m*m)*(m*m)*m;
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = pi * denom * denom;

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometrySchlickGGX(NdotV, roughness);
    float ggx1  = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

#endif// ATTA_RAST_MATERIALS_BASE_GLSL
