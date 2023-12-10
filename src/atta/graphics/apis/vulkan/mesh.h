//--------------------------------------------------
// Atta Graphics Module
// mesh.h
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_MESH_H
#define ATTA_GRAPHICS_APIS_VULKAN_MESH_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/indexBuffer.h>
#include <atta/graphics/apis/vulkan/vertexBuffer.h>
#include <atta/graphics/mesh.h>

namespace atta::graphics::vk {

class Mesh final : public gfx::Mesh {
  public:
    Mesh(CreateInfo info);
    ~Mesh();

    void draw() override;
    void draw(VkCommandBuffer commandBuffer);
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_MESH_H
