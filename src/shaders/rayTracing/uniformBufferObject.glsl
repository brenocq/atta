struct UniformBufferObject
{
	mat4 viewMat;
	mat4 projMat;
	mat4 viewMatInverse;
	mat4 projMatInverse;

	uint samplesPerFrame;
	uint totalNumberOfSamples;
	uint numberOfBounces;
};
