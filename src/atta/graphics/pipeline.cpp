//--------------------------------------------------
// Atta Graphics Module
// pipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/pipeline.h>

namespace atta::graphics {

Pipeline::Pipeline(const CreateInfo& info)
    : _shader(info.shader), _renderPass(info.renderPass), _primitiveTopology(info.primitiveTopology), _backfaceCulling(info.backfaceCulling),
      _wireframe(info.wireframe), _lineWidth(info.lineWidth), _debugName(info.debugName) {}

void Pipeline::setBool(const char* name, bool b) { _shader->setBool(name, b); }
void Pipeline::setInt(const char* name, int i) { _shader->setInt(name, i); }
void Pipeline::setFloat(const char* name, float f) { _shader->setFloat(name, f); }
void Pipeline::setVec2(const char* name, vec2 v) { _shader->setVec2(name, v); }
void Pipeline::setVec3(const char* name, vec3 v) { _shader->setVec3(name, v); }
void Pipeline::setVec4(const char* name, vec4 v) { _shader->setVec4(name, v); }
void Pipeline::setMat3(const char* name, mat3 m) { _shader->setMat3(name, m); }
void Pipeline::setMat4(const char* name, mat4 m) { _shader->setMat4(name, m); }
void Pipeline::setImage(const char* name, StringId sid) { _shader->setImage(name, sid); }
void Pipeline::setImage(const char* name, std::shared_ptr<Image> image) { _shader->setImage(name, image); }
void Pipeline::setCubemap(const char* name, StringId sid) { _shader->setCubemap(name, sid); }
void Pipeline::setCubemap(const char* name, std::shared_ptr<Image> image) { _shader->setCubemap(name, image); }

void Pipeline::renderMesh(StringId meshSid) { LOG_WARN("Pipeline", "[w]renderMesh[] was not implemented yet"); }
void Pipeline::renderQuad() { LOG_WARN("Pipeline", "[w]renderQuad[] was not implemented yet"); }
void Pipeline::renderQuad3() { LOG_WARN("Pipeline", "[w]renderQuad3[] was not implemented yet"); }
void Pipeline::renderCube() { LOG_WARN("Pipeline", "[w]renderCube[] was not implemented yet"); }

std::shared_ptr<Shader> Pipeline::getShader() const { return _shader; };
std::shared_ptr<RenderPass> Pipeline::getRenderPass() const { return _renderPass; };
StringId Pipeline::getDebugName() const { return _debugName; }

} // namespace atta::graphics
