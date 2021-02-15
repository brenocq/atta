//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// vertex.glsl
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
struct Vertex
{
  vec3 position;
  vec3 normal;
  vec2 texCoord;
  int materialIndex;
};

Vertex unpackVertex(uint index)
{
	const uint vertexSize = 9;
	const uint offset = index * vertexSize;

	Vertex v;

	v.position = vec3(Vertices[offset + 0], Vertices[offset + 1], Vertices[offset + 2]);
	v.normal = vec3(Vertices[offset + 3], Vertices[offset + 4], Vertices[offset + 5]);
	v.texCoord = vec2(Vertices[offset + 6], Vertices[offset + 7]);
	v.materialIndex = floatBitsToInt(Vertices[offset + 8]);

	return v;
}
