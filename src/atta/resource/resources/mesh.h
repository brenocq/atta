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
    enum class Type { VEC2 = 0, VEC3, VEC4 };
    struct VertexElement {
        Type type;
        std::string name;
    };
    using VertexLayout = std::vector<VertexElement>;
    using Index = uint32_t;

    Mesh(const fs::path& filename);

    const std::vector<uint8_t>& getVertices() const { return _vertices; };
    const std::vector<Index>& getIndices() const { return _indices; };
    const VertexLayout& getVertexLayout() const { return _vertexLayout; }

  private:
    void load();

    // Assimp mesh loading
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);

    /// Draw vertex data following the format specified in the vertex layout
    std::vector<uint8_t> _vertices;
    unsigned _numVertices;
    VertexLayout _vertexLayout;
    std::vector<Index> _indices;
};

} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCES_MESH_H
