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
    pipelineInfo.shader = gfx::create<Shader>("shaders/common/grid.asl");
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.primitive = Pipeline::Primitive::LINE;
    pipelineInfo.debugName = StringId("Grid Pipeline");
    _pipeline = gfx::create<Pipeline>(pipelineInfo);

    // Create line mesh
    _gridMeshName = "atta::gfx::GridPipeline[" + std::to_string(_gridId++) + "]";
    res::Mesh::CreateInfo meshInfo{};
    uint8_t* data = (uint8_t*)_lines.data();
    size_t size = _lines.size() * sizeof(Line);
    meshInfo.vertices = std::vector<uint8_t>(data, data + size);
    meshInfo.vertexLayout.push_back({resource::Mesh::VertexElement::VEC3, "iPos"});
    meshInfo.vertexLayout.push_back({resource::Mesh::VertexElement::VEC4, "iColor"});
    meshInfo.vertexLayout.push_back({resource::Mesh::VertexElement::FLOAT, "iWidth"});
    res::create<res::Mesh>(_gridMeshName, meshInfo);
}

GridPipeline::~GridPipeline() { res::destroy<res::Mesh>(_gridMeshName); }

void GridPipeline::update(std::shared_ptr<Camera> camera) {
    // Update grid
    {
        int i = 0;
        int size = 50;
        vec2i camPos = camera->getPosition();
        // Create grid
        for (int j = -size; j <= size; j++) {
            _lines[i++].pos = {float(j + camPos.x), float(camPos.y - size), 0.0f};
            _lines[i++].pos = {float(j + camPos.x), float(size + camPos.y), 0.0f};
            _lines[i++].pos = {float(camPos.x - size), float(j + camPos.y), 0.0f};
            _lines[i++].pos = {float(size + camPos.x), float(j + camPos.y), 0.0f};
        }

        for (int j = 0; j < i; j += 2) {
            // Default line style
            float width = 1.0f;
            vec4 color = {1, 1, 1, 0.1};

            // Line style every 10 meters
            if ((int(_lines[j].pos.y) % 10 == 0 && _lines[j].pos.y == _lines[j + 1].pos.y) ||
                (int(_lines[j].pos.x) % 10 == 0 && _lines[j].pos.x == _lines[j + 1].pos.x))
                color = {1, 1, 1, 0.2};

            // Line style of X and Y axes
            if (_lines[j].pos.y == 0.0f && _lines[j + 1].pos.y == 0.0f)
                color = {0.6, 0.2, 0.2, 1.0f};
            else if (_lines[j].pos.x == 0.0f && _lines[j + 1].pos.x == 0.0f)
                color = {0.2, 0.6, 0.2, 1.0f};

            // Update line style
            _lines[j].color = _lines[j + 1].color = color;
            _lines[j].width = _lines[j + 1].width = width;
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
        _pipeline->setVec3("uCamPos", camera->getPosition());
        _pipeline->renderMesh(_gridMeshName, _numLines);
    }
    _pipeline->end();
}

} // namespace atta::graphics
