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
    struct VertexElement {
        enum Type { VEC2 = 0, VEC3, VEC4 };
        Type type;
        std::string name;
    };
    using VertexLayout = std::vector<VertexElement>;
    using Index = uint32_t;

    struct CreateInfo {
        std::vector<uint8_t> vertices;
        VertexLayout vertexLayout;
        std::vector<Index> indices;
    };

    Mesh(const fs::path& filename);
    Mesh(const fs::path& filename, const CreateInfo& info);

    void updateVertices(const std::vector<uint8_t>& vertices); ///< Update vertex data

    const std::vector<uint8_t>& getVertices() const;
    const std::vector<Index>& getIndices() const;
    const VertexLayout& getVertexLayout() const;

  private:
    void update() const;

    // Assimp mesh loading
    void load();
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    unsigned _assimpVertexIdx; ///< Used only to keep track of indices while processing the meshes

    /// Draw vertex data following the format specified in the vertex layout
    std::vector<uint8_t> _vertices;
    VertexLayout _vertexLayout;
    std::vector<Index> _indices;
};

} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCES_MESH_H
