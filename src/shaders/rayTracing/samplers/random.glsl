//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// random.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef RANDOM_GLSL
#define RANDOM_GLSL
const float piOver4 = 3.14159265358979f*0.25f;
const float piOver2 = 3.14159265358979f*0.5f;

// Generate a random unsigned int from two unsigned int values, using 16 pairs
// of rounds of the Tiny Encryption Algorithm. See Zafar, Olano, and Curtis,
// "GPU Random Numbers via the Tiny Encryption Algorithm"
uint tea(uint val0, uint val1)
{
  uint v0 = val0;
  uint v1 = val1;
  uint s0 = 0;

  for(uint n = 0; n < 16; n++)
  {
    s0 += 0x9e3779b9;
    v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
    v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
  }

  return v0;
}

// Generate a random unsigned int in [0, 2^24) given the previous RNG state
// using the Numerical Recipes linear congruential generator (LCG)
uint randomUInt(inout uint prev)
{
  uint LCG_A = 1664525u;
  uint LCG_C = 1013904223u;
  prev       = (LCG_A * prev + LCG_C);
  return prev & 0x00FFFFFF;
}

// Generate a random float in [0, 1) given the previous RNG state
float randomFloat(inout uint prev)
{
  return (float(randomUInt(prev)) / float(0x01000000));
}

vec2 concentricSampleDisk(vec2 u)
{
	// Map [0,1)^2 -> [-1,1]^2
	vec2 uOffset = 2.f*u-vec2(1,1);

	// Handle degeneracy at the origin
	if(uOffset.x == 0 && uOffset.y == 0)
		return vec2(0, 0);

	// Apply concentric mapping to point
	float theta, r;
	if(abs(uOffset.x) > abs(uOffset.y))
    {
       r = uOffset.x;
       theta = piOver4 * (uOffset.y / uOffset.x);
    }
    else
    {
        r = uOffset.y;
        theta = piOver2 - piOver4 * (uOffset.x / uOffset.y);
    }

	return r * vec2(cos(theta), sin(theta));
}

#endif// RANDOM_GLSL
