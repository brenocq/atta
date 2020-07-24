#extension GL_EXT_control_flow_attributes : require

// Generates a seed for a random number generator from 2 inputs plus a backoff
// https://github.com/nvpro-samples/optix_prime_baking/blob/master/random.h
// https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
uint initRandomSeed(uint val0, uint val1)
{
	uint v0 = val0, v1 = val1, s0 = 0;

	[[unroll]] 
	for(uint n = 0; n < 16; n++)
	{
		s0 += 0x9e3779b9;
		v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
		v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
	}

	return v0;
}

uint randomInt(inout uint seed)
{
	// LCG values from Numerical Recipes
    return (seed = 1664525 * seed + 1013904223);
}

float randomFloat(inout uint seed)
{
	// Float version using bitmask from Numerical Recipes
	const uint one = 0x3f800000;
	const uint msk = 0x007fffff;
	return uintBitsToFloat(one | (msk & (randomInt(seed) >> 9))) - 1;
}

vec2 randomInUnitDisk(inout uint seed)
{
	for(;;)
	{
		const vec2 p = 2 * vec2(randomFloat(seed), randomFloat(seed)) - 1;
		if(dot(p, p) < 1)
		{
			return p;
		}
	}
}

vec3 randomInUnitSphere(inout uint seed)
{
	for(;;)
	{
		const vec3 p = 2 * vec3(randomFloat(seed), randomFloat(seed), randomFloat(seed)) - 1;
		if(dot(p, p) < 1)
		{
			return p;
		}
	}
}
