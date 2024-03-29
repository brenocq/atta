//--------------------------------------------------
// Atta Resource Module
// mesh.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <atta/file/manager.h>
#include <atta/resource/resources/mesh.h>

namespace atta::resource {

Mesh::Mesh(const fs::path& filename) : Resource(filename) { load(); }

//---------- Assimp mesh loading ----------//
void Mesh::load() {
    fs::path absolutePath = file::solveResourcePath(_filename);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(absolutePath.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("resource::Mesh", "Failed to load mesh [w]$0. Assimp Error: $1", fs::absolute(_filename).string(), importer.GetErrorString());
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Mesh::processNode(aiNode* node, const aiScene* scene) {
    // LOG_DEBUG("resource::Mesh", "Process node $0", std::string(node->mName.C_Str()));

    // Process all the node's meshes (if any)
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
        // LOG_DEBUG("resource::Mesh", " - Process mesh $0", std::string(mesh->mName.C_Str()));
    }

    // Then do the same for each of its children
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Mesh::processMesh(aiMesh* mesh, const aiScene* scene) {
    unsigned startIndex = _vertices.size();
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // Process vertex positions, normals and texture coordinates
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

        // Check if there are texture coords
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        } else
            vertex.texCoord = vec2(0.0f, 0.0f);

        _vertices.push_back(vertex);
    }
    // Process indices
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            _indices.push_back(startIndex + face.mIndices[j]);
    }
}

} // namespace atta::resource
