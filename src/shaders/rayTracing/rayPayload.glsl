struct RayPayload
{
	vec4 colorAndDistance;// rgb + t
	vec4 scatterDirection;// xyz + w (is scatter needed)
	uint randomSeed;
};
