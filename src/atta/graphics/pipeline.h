//--------------------------------------------------
// Atta Graphics Module
// pipeline.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_PIPELINE_H
#define ATTA_GRAPHICS_PIPELINE_H

#include <atta/graphics/renderPass.h>
#include <atta/graphics/shader.h>
#include <atta/graphics/vertexBuffer.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Pipeline {
  public:
    enum PrimitiveTopology { NONE, POINT, LINE, TRIANGLE, LINE_STRIP, TRIANGLE_STRIP, TRIANGLE_FAN };

    struct CreateInfo {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<RenderPass> renderPass;
        PrimitiveTopology primitiveTopology = PrimitiveTopology::TRIANGLE;
        bool backfaceCulling = true;
        bool wireframe = false;
        bool lineWidth = 1.0f;

        StringId debugName = StringId("Unnamed Pipeline");
    };

    Pipeline(const CreateInfo& info);
    virtual ~Pipeline() = default;

    virtual void begin(std::shared_ptr<RenderQueue> renderQueue) = 0;
    virtual void end() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual void* getImGuiTexture() const = 0;

    void setBool(const char* name, bool b);
    void setInt(const char* name, int i);
    void setFloat(const char* name, float f);
    void setVec2(const char* name, vec2 v);
    void setVec3(const char* name, vec3 v);
    void setVec4(const char* name, vec4 v);
    void setMat3(const char* name, mat3 m);
    void setMat4(const char* name, mat4 m);
    void setImage(const char* name, StringId sid);
    void setImage(const char* name, std::shared_ptr<Image> image);
    void setCubemap(const char* name, StringId sid);
    void setCubemap(const char* name, std::shared_ptr<Image> image);

    virtual void renderMesh(StringId meshSid);
    virtual void renderQuad();
    virtual void renderQuad3();
    virtual void renderCube();

    std::shared_ptr<Shader> getShader() const;
    std::shared_ptr<RenderPass> getRenderPass() const;
    StringId getDebugName() const;

  protected:
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<RenderQueue> _renderQueue;

    PrimitiveTopology _primitiveTopology;

    const bool _backfaceCulling;
    const bool _wireframe;
    const bool _lineWidth;

    const StringId _debugName;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_PIPELINE_H
