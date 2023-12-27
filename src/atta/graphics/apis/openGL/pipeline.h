//--------------------------------------------------
// Atta Graphics Module
// pipeline.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_PIPELINE_H
#define ATTA_GRAPHICS_APIS_OPENGL_PIPELINE_H

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics::gl {

class Pipeline final : public gfx::Pipeline {
  public:
    Pipeline(const Pipeline::CreateInfo& info);
    ~Pipeline();

    void begin(std::shared_ptr<RenderQueue> renderQueue) override;
    void end() override;

    void resize(uint32_t width, uint32_t height) override;

    void renderMesh(StringId meshSid) override;

    void* getImGuiTexture() const override;

    void createImageGroup(ImageGroupType type, std::string name) override;
    void updateImageGroup(std::string name, ImageGroup imageGroup) override;
    void destroyImageGroup(std::string name) override;

    void setImageGroup(const char* name) override;

  private:
    std::map<StringId, ImageGroup> _imageGroups;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_PIPELINE_H
