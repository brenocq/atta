// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics::gl {

class Pipeline final : public gfx::Pipeline {
  public:
    Pipeline(const Pipeline::CreateInfo& info);
    ~Pipeline();

    void begin() override;
    void end() override;

    void resize(uint32_t width, uint32_t height) override;

    void renderMesh(StringId meshSid, size_t numVertices = 0) override;

    void* getImGuiTexture() const override;

    void createImageGroup(ImageGroupType type, std::string name) override;
    void updateImageGroup(std::string name, ImageGroup imageGroup) override;
    void destroyImageGroup(std::string name) override;

    void setImageGroup(const char* name) override;

  private:
    std::map<StringId, ImageGroup> _imageGroups;
};

} // namespace atta::graphics::gl
