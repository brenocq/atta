//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// uniformBufferObject.glsl
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
struct UniformBufferObject
{
	mat4 viewMat;
	mat4 projMat;
	mat4 viewMatInverse;
	mat4 projMatInverse;

	uint samplesPerPixel;//uint samplesPerPixel
	uint nAccSamples;// Number of accumulated samples
	
	uint maxDepth;

	uint seed;
};
