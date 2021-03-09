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

#endif// ATTA_RAST_MATERIALS_BASE_GLSL
