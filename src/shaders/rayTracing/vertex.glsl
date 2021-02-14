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
