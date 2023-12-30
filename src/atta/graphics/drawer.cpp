//--------------------------------------------------
// Atta Graphics Module
// drawer.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/drawer.h>
#include <atta/resource/interface.h>

namespace atta::graphics {

Drawer::Drawer()
    : _maxNumberOfLines(100000), _currNumberOfLines(0), _linesChanged(false), _maxNumberOfPoints(1000000), _currNumberOfPoints(0),
      _pointsChanged(false) {
    _lines.resize(_maxNumberOfLines);
    _points.resize(_maxNumberOfPoints);
    // Line buffer
    {
        res::Mesh::CreateInfo info{};
        uint8_t* data = (uint8_t*)_lines.data();
        size_t size = _lines.size() * sizeof(Line);
        info.vertices = std::vector<uint8_t>(data, data + size);
        info.vertexLayout.push_back({resource::Mesh::VertexElement::VEC3, "iPos"});
        info.vertexLayout.push_back({resource::Mesh::VertexElement::VEC4, "iColor"});
        res::create<res::Mesh>("atta::gfx::Drawer::lines", info);
    }

    // Point buffer
    {
        res::Mesh::CreateInfo info{};
        uint8_t* data = (uint8_t*)_points.data();
        size_t size = _points.size() * sizeof(Point);
        info.vertices = std::vector<uint8_t>(data, data + size);
        info.vertexLayout.push_back({resource::Mesh::VertexElement::VEC3, "iPos"});
        info.vertexLayout.push_back({resource::Mesh::VertexElement::VEC4, "iColor"});
        res::create<res::Mesh>("atta::gfx::Drawer::points", info);
    }
}

Drawer& Drawer::getInstance() {
    static Drawer drawer;
    return drawer;
}

void Drawer::clear(StringId group) { getInstance().clearImpl(group); }
void Drawer::clearImpl(StringId group) {
    if (group == "No group"_sid) {
        _lineGroups.clear();
        _pointGroups.clear();
        _linesChanged = true;
        _pointsChanged = true;
    } else {
        clearImpl<Line>(group);
        clearImpl<Point>(group);
    }
}

} // namespace atta::graphics
