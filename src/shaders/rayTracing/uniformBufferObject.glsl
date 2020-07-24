struct UniformBufferObject
{
	mat4 modelView;
	mat4 projection;
	mat4 modelViewInverse;
	mat4 projectionInverse;
	float aperture;
	float focusDistance;
	uint totalNumberOfSamples;
	uint numberOfSamples;
	uint numberOfBounces;
	uint randomSeed;
	bool gammaCorrection;
	bool hasSky;
};
