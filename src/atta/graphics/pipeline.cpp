//--------------------------------------------------
// Atta Graphics Module
// pipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/graphics/pipeline.h>

#include <atta/resource/interface.h>

#include <atta/event/events/materialCreate.h>
#include <atta/event/events/materialDestroy.h>
#include <atta/event/events/materialUpdate.h>

namespace atta::graphics {

Pipeline::Pipeline(const CreateInfo& info)
    : _shader(info.shader), _renderPass(info.renderPass), _primitive(info.primitive), _backfaceCulling(info.backfaceCulling),
      _wireframe(info.wireframe), _lineWidth(info.lineWidth), _enableDepthTest(info.enableDepthTest), _debugName(info.debugName) {
    //---------- Track material update ----------//
    event::subscribe<event::MaterialCreate>(BIND_EVENT_FUNC(Pipeline::onMaterialCreate));
    event::subscribe<event::MaterialDestroy>(BIND_EVENT_FUNC(Pipeline::onMaterialDestroy));
    event::subscribe<event::MaterialUpdate>(BIND_EVENT_FUNC(Pipeline::onMaterialUpdate));
    for (StringId materialSid : resource::getResources<resource::Material>()) {
        _imageGroupsToCreate.insert(materialSid);
        _imageGroupsToUpdate.insert(materialSid);
    }
}

Pipeline::~Pipeline() {
    event::unsubscribe<event::MaterialCreate>(BIND_EVENT_FUNC(Pipeline::onMaterialCreate));
    event::unsubscribe<event::MaterialDestroy>(BIND_EVENT_FUNC(Pipeline::onMaterialDestroy));
    event::unsubscribe<event::MaterialUpdate>(BIND_EVENT_FUNC(Pipeline::onMaterialUpdate));
}

void Pipeline::setBool(const char* name, bool b) { _shader->setBool(name, b); }
void Pipeline::setInt(const char* name, int i) { _shader->setInt(name, i); }
void Pipeline::setFloat(const char* name, float f) { _shader->setFloat(name, f); }
void Pipeline::setVec2(const char* name, vec2 v) { _shader->setVec2(name, v); }
void Pipeline::setVec3(const char* name, vec3 v) { _shader->setVec3(name, v); }
void Pipeline::setVec4(const char* name, vec4 v) { _shader->setVec4(name, v); }
void Pipeline::setMat3(const char* name, mat3 m) { _shader->setMat3(name, m); }
void Pipeline::setMat4(const char* name, mat4 m) { _shader->setMat4(name, m); }
void Pipeline::setImageGroup(std::string name) { setImageGroup(name.c_str()); }
void Pipeline::setImageGroup(StringId name) { setImageGroup(name.getString().c_str()); }

void Pipeline::renderMesh(StringId meshSid, size_t numVertices) { LOG_WARN("Pipeline", "[w]renderMesh[] was not implemented yet"); }
void Pipeline::renderQuad() { LOG_WARN("Pipeline", "[w]renderQuad[] was not implemented yet"); }
void Pipeline::renderQuad3() { LOG_WARN("Pipeline", "[w]renderQuad3[] was not implemented yet"); }
void Pipeline::renderCube() { LOG_WARN("Pipeline", "[w]renderCube[] was not implemented yet"); }

std::shared_ptr<Shader> Pipeline::getShader() const { return _shader; };
std::shared_ptr<RenderPass> Pipeline::getRenderPass() const { return _renderPass; };
StringId Pipeline::getDebugName() const { return _debugName; }

Pipeline::ImageGroupItem::ImageGroupItem(std::string name_, std::shared_ptr<Image> image_) : name(name_), image(image_) {}
Pipeline::ImageGroupItem::ImageGroupItem(std::string name_, StringId image_) : name(name_) {
    const auto& images = gfx::Manager::getInstance().getImages();
    if (images.find(image_) == images.end()) {
        LOG_WARN("gfx::Pipeline::ImageGroupItem", "Could not find image [w]$0[]", image_);
        return;
    }
    image = images.at(image_);
}

void Pipeline::updateImageGroupsFromMaterials(ImageGroupFromMaterialFunc func) {
    for (StringId sid : _imageGroupsToDestroy)
        destroyImageGroup(sid.getString());
    for (StringId sid : _imageGroupsToCreate)
        createImageGroup(Pipeline::ImageGroupType::PER_DRAW, sid.getString());
    for (StringId sid : _imageGroupsToUpdate) {
        resource::Material* m = resource::get<resource::Material>(sid);
        updateImageGroup(sid.getString(), func(m));
    }
    _imageGroupsToDestroy.clear();
    _imageGroupsToUpdate.clear();
    _imageGroupsToCreate.clear();
}

void Pipeline::onMaterialCreate(event::Event& event) {
    event::MaterialCreate& e = reinterpret_cast<event::MaterialCreate&>(event);
    _imageGroupsToCreate.insert(e.sid);
    _imageGroupsToUpdate.insert(e.sid);
}

void Pipeline::onMaterialDestroy(event::Event& event) {
    event::MaterialDestroy& e = reinterpret_cast<event::MaterialDestroy&>(event);
    _imageGroupsToDestroy.insert(e.sid);
    _imageGroupsToCreate.erase(e.sid);
    _imageGroupsToUpdate.erase(e.sid);
}

void Pipeline::onMaterialUpdate(event::Event& event) {
    event::MaterialUpdate& e = reinterpret_cast<event::MaterialUpdate&>(event);
    _imageGroupsToUpdate.insert(e.sid);
}

} // namespace atta::graphics
