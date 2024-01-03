//--------------------------------------------------
// Atta Graphics Module
// gridPipeline.cpp
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/graphics/renderers/common/gridPipeline.h>
#include <atta/resource/interface.h>

namespace atta::graphics {

size_t GridPipeline::_gridId = 0;

GridPipeline::GridPipeline(std::shared_ptr<RenderPass> renderPass) : _numLines(0) {
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = gfx::create<Shader>("shaders/grid/grid.asl");
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.primitive = Pipeline::Primitive::LINE;
    pipelineInfo.debugName = StringId("Grid Pipeline");
    _pipeline = gfx::create<Pipeline>(pipelineInfo);

    // Create line mesh
    _gridMeshName = "atta::gfx::GridPipeline[" + std::to_string(_gridId++) + "]";
    LOG_DEBUG("GridPipeline", "Mesh id $0", _gridMeshName);
    res::Mesh::CreateInfo meshInfo{};
    uint8_t* data = (uint8_t*)_lines.data();
    size_t size = _lines.size() * sizeof(Line);
    meshInfo.vertices = std::vector<uint8_t>(data, data + size);
    meshInfo.vertexLayout.push_back({resource::Mesh::VertexElement::VEC3, "iPos"});
    meshInfo.vertexLayout.push_back({resource::Mesh::VertexElement::VEC4, "iColor"});
    meshInfo.vertexLayout.push_back({resource::Mesh::VertexElement::FLOAT, "iWidth"});
    res::create<res::Mesh>(_gridMeshName, meshInfo);
}

GridPipeline::~GridPipeline() {
    // TODO Delete grid
}

void GridPipeline::update(std::shared_ptr<Camera> camera) {
    // Update grid
    {
        int i = 0;
        int size = 100;
        for (int j = -size; j <= size; j++) {
            _lines[i++].pos = {float(j), -float(size), 0.0f};
            _lines[i++].pos = {float(j), float(size), 0.0f};
            _lines[i++].pos = {-float(size), float(j), 0.0f};
            _lines[i++].pos = {float(size), float(j), 0.0f};
        }

        for (int j = 0; j < i; j++) {
            float width = 1.0f;
            vec4 color = {1, 1, 1, 0.2};
            if ((j / 4) % 10 == 0)
                color = {1, 1, 1, 0.4};

            if (_lines[j].pos.y == 0.0f)
                _lines[j].color = {0.6, 0.2, 0.2, 1.0f};
            else if (_lines[j].pos.x == 0.0f)
                _lines[j].color = {0.2, 0.6, 0.2, 1.0f};
            else
                _lines[j].color = color;
            _lines[j].width = width;
        }
        _numLines = i;
    }

    // Update mesh with grid
    uint8_t* data = (uint8_t*)_lines.data();
    size_t size = _lines.size() * sizeof(Line);
    res::get<res::Mesh>(_gridMeshName)->updateVertices(std::vector<uint8_t>(data, data + size));
}

void GridPipeline::render(std::shared_ptr<Camera> camera) {
    if (_numLines == 0)
        return;

    _pipeline->begin();
    {
        _pipeline->setMat4("uProjection", camera->getProj());
        _pipeline->setMat4("uView", camera->getView());
        _pipeline->renderMesh(_gridMeshName, _numLines);
    }
    _pipeline->end();
}

} // namespace atta::graphics
