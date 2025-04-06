//--------------------------------------------------
// Atta Graphics Module
// pipeline.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_PIPELINE_H
#define ATTA_GRAPHICS_PIPELINE_H

#include <atta/event/event.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/shader.h>
#include <atta/graphics/vertexBuffer.h>
#include <atta/resource/resources/material.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Pipeline {
  public:
    enum Primitive { POINT = 0, LINE, TRIANGLE };

    struct CreateInfo {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<RenderPass> renderPass;
        Primitive primitive = Primitive::TRIANGLE;
        bool backfaceCulling = true;
        bool wireframe = false;
        bool lineWidth = 1.0f;
        bool enableDepthTest = true; // Can be set to false to disable depth testing for this pipeline

        StringId debugName = StringId("Unnamed Pipeline");
    };

    Pipeline(const CreateInfo& info);
    virtual ~Pipeline();

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual void* getImGuiTexture() const = 0;

    enum class ImageGroupType { PER_FRAME = 0, PER_DRAW };
    struct ImageGroupItem {
        ImageGroupItem(std::string name_, std::shared_ptr<Image> image_);
        ImageGroupItem(std::string name_, StringId image_);

        std::string name;
        std::shared_ptr<Image> image;
    };
    using ImageGroup = std::vector<ImageGroupItem>;
    /// Image groups should be created to update images, they are grouped by perFrame and perDraw
    virtual void createImageGroup(ImageGroupType type, std::string name) = 0;
    virtual void updateImageGroup(std::string name, ImageGroup imageGroup) = 0;
    virtual void destroyImageGroup(std::string name) = 0;

    using ImageGroupFromMaterialFunc = std::function<ImageGroup(resource::Material*)>;
    void updateImageGroupsFromMaterials(ImageGroupFromMaterialFunc func);

    void setBool(const char* name, bool b);
    void setInt(const char* name, int i);
    void setFloat(const char* name, float f);
    void setVec2(const char* name, vec2 v);
    void setVec3(const char* name, vec3 v);
    void setVec4(const char* name, vec4 v);
    void setMat3(const char* name, mat3 m);
    void setMat4(const char* name, mat4 m);
    void setImageGroup(std::string name);
    void setImageGroup(StringId name);
    virtual void setImageGroup(const char* name) = 0;

    /**
     * @brief Render mesh
     *
     * @note If numVertices is set to zero, all vertices are rendered
     * @warning numVertices should only be used if there is no index buffer
     */
    virtual void renderMesh(StringId meshSid, size_t numVertices = 0);
    virtual void renderQuad();
    virtual void renderQuad3();
    virtual void renderCube();

    std::shared_ptr<Shader> getShader() const;
    std::shared_ptr<RenderPass> getRenderPass() const;
    StringId getDebugName() const;

  protected:
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<RenderPass> _renderPass;

    Primitive _primitive;

    const bool _backfaceCulling;
    const bool _wireframe;
    const bool _lineWidth;
    const bool _enableDepthTest;

    const StringId _debugName;

  private:
    void onMaterialCreate(event::Event& event);
    void onMaterialDestroy(event::Event& event);
    void onMaterialUpdate(event::Event& event);

    // Helper to update image groups from materials
    std::set<StringId> _imageGroupsToCreate;
    std::set<StringId> _imageGroupsToUpdate;
    std::set<StringId> _imageGroupsToDestroy;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_PIPELINE_H
