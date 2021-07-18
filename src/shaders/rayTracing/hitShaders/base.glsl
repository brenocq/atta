//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// base.glsl
// Date: 2021-07-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef HIT_SHADERS_BASE_GLSL
#define HIT_SHADERS_BASE_GLSL

struct Interaction
{
	vec3 ng;// Geometric normal
	vec3 ns;// Shading normal
	vec3 ts;// Shading orthonormal base vector
	vec3 ss;// Shading orthonormal base vector

	uint materialIndex;
	vec2 texCoord;

	vec3 worldPos;
	vec3 wo;
};

vec2 mixCoords(vec2 a, vec2 b, vec2 c, vec3 barycentrics)
{
	return a * barycentrics.x + b * barycentrics.y + c * barycentrics.z;
}

vec3 mixCoords(vec3 a, vec3 b, vec3 c, vec3 barycentrics) 
{
    return a * barycentrics.x + b * barycentrics.y + c * barycentrics.z;
}

void chooseBase(vec3 normal, out vec3 ts, out vec3 ss)
{
	// Choose best vector to create orthonormal base
	float absx = abs(normal.x), absy = abs(normal.y), absz = abs(normal.z);
	if(absz<absy && absz<absx)
		ss = cross(vec3(0,0,1), normal);
	else if(absy<absx)
		ss = cross(vec3(0,1,0), normal);
	else
		ss = cross(vec3(1,0,0), normal);

	// Create orthonormal base
	ts = cross(normal, ss);
	ss = cross(normal, ts);
}

Interaction populateInteraction()
{
	Interaction it;

	// Get primitive and object data
	const ObjectInfo object = Objects[gl_InstanceID];
	const uint indexOffset = object.indexOffset;
	const uint vertexOffset = object.vertexOffset;
	const Vertex v0 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 0]);
	const Vertex v1 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 1]);
	const Vertex v2 = unpackVertex(vertexOffset + Indices[indexOffset + gl_PrimitiveID * 3 + 2]);

	// Get material
	it.materialIndex = object.materialOffset+v0.materialIndex;
	// Compute the ray hit point properties
	const vec3 barycentrics = vec3(1.0 - hitAttributes.x - hitAttributes.y, hitAttributes.x, hitAttributes.y);
	// Compute normals
	it.ng = mixCoords(v0.normal, v1.normal, v2.normal, barycentrics);
	it.ng = normalize(vec3(inverse(transpose(object.transform))*vec4(it.ng, 0.0)));
	it.ns = it.ng;
	// Compute normal base
	chooseBase(it.ns, it.ts, it.ss);
	// Compute texCoord
	it.texCoord = mixCoords(v0.texCoord, v1.texCoord, v2.texCoord, barycentrics);

	// Compute world pos
	it.worldPos = mixCoords(v0.position, v1.position, v2.position, barycentrics);
	it.worldPos = vec3(object.transform*vec4(it.worldPos, 1.0f));
	// Compute outgoing direction
	it.wo = -gl_WorldRayDirectionEXT;

	return it;
}


#endif// HIT_SHADERS_BASE_GLSL
