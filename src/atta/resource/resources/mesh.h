//--------------------------------------------------
// Atta Resource Module
// mesh.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_RESOURCES_MESH_H
#define ATTA_RESOURCE_RESOURCES_MESH_H

#include <atta/memory/allocatedObject.h>
#include <atta/resource/resource.h>
#include <atta/utils/math/vector.h>

struct aiNode;
struct aiMesh;
struct aiScene;

namespace atta::resource {

class Mesh : public Resource, public memory::AllocatedObject<Mesh, SID("ResourceAllocator")> {
  public:
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texCoord;
    };

    Mesh(const fs::path& filename);

    const std::vector<Vertex>& getVertices() const { return _vertices; };
    const std::vector<uint32_t>& getIndices() const { return _indices; };

  private:
    void load();

    // Assimp mesh loading
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;
};

} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCES_MESH_H
