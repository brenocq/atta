//--------------------------------------------------
// Atta Graphics Module
// pipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/image.h>
#include <atta/graphics/apis/openGL/pipeline.h>
#include <atta/graphics/apis/openGL/shader.h>
#include <atta/graphics/interface.h>

namespace atta::graphics::gl {

Pipeline::Pipeline(const Pipeline::CreateInfo& info) : gfx::Pipeline(info) {
    DASSERT(_renderPass, "Can not create pipeline without render pass");
    DASSERT(_shader, "Can not create pipeline without shader group");
}

Pipeline::~Pipeline() {}

void Pipeline::begin(std::shared_ptr<RenderQueue> renderQueue) { _shader->bind(); }

void Pipeline::end() { _shader->unbind(); }

void Pipeline::resize(uint32_t width, uint32_t height) { _renderPass->getFramebuffer()->resize(width, height); }

void Pipeline::renderMesh(StringId meshSid) {
    std::shared_ptr<Mesh> mesh = Manager::getInstance().getMeshes().at(meshSid);
    if (mesh)
        mesh->draw();
    else
        LOG_WARN("gfx::gl::Pipeline", "Could not render mesh [w]$0[], mesh not found", meshSid);
}

void* Pipeline::getImGuiTexture() const {
    return reinterpret_cast<void*>(std::static_pointer_cast<gl::Image>(_renderPass->getFramebuffer()->getImage(0))->getImGuiImage());
}

void Pipeline::createImageGroup(ImageGroupType type, std::string name) { _imageGroups[name] = {}; }
void Pipeline::updateImageGroup(std::string name, ImageGroup imageGroup) { _imageGroups[name] = imageGroup; }
void Pipeline::destroyImageGroup(std::string name) { _imageGroups.erase(name); }

void Pipeline::setImageGroup(const char* name) {
    if (_imageGroups.find(StringId(name)) == _imageGroups.end()) {
        LOG_WARN("gfx::gl::Pipeline", "Could not set image group [w]$0[] that was not created", name);
        return;
    }

    for (const ImageGroupItem& item : _imageGroups.at(name))
        std::static_pointer_cast<gl::Shader>(_shader)->setImage(item.name.c_str(), item.image);
}

} // namespace atta::graphics::gl
