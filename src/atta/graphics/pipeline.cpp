//--------------------------------------------------
// Atta Graphics Module
// pipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/pipeline.h>

namespace atta::graphics {

Pipeline::Pipeline(const CreateInfo& info)
    : _shaderGroup(info.shaderGroup), _renderPass(info.renderPass), _layout(info.layout), _primitiveTopology(info.primitiveTopology),
      _backfaceCulling(info.backfaceCulling), _wireframe(info.wireframe), _lineWidth(info.lineWidth), _debugName(info.debugName) {}

std::shared_ptr<Shader> Pipeline::getShader() const { return _shader; };
std::shared_ptr<RenderPass> Pipeline::getRenderPass() const { return _renderPass; };
StringId Pipeline::getDebugName() const { return _debugName; }

} // namespace atta::graphics
