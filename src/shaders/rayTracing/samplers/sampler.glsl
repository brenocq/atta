const float epsilon = 0.00001;
const float oneMinusEpsilon = 1-epsilon;

#include "random.glsl"

// Seed from pixel position (y*width+x)
// The sampler generated a vector of samples uniformly distributed
// { 2D(film), 2D(lens) }


// Book implementations
//  - Stratified Sampler
//  - Zero Two Sequence Sampler

// TODO implement samplers to create better samples distributions, for now I`m using only
// a linear congruential generator (LCG)
