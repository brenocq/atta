//--------------------------------------------------
// Atta Graphics Module
// drawer.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/drawer.h>
#include <atta/graphics/interface.h>

namespace atta::graphics {

Drawer::Drawer()
    : _maxNumberOfLines(100000), _currNumberOfLines(0), _linesChanged(false), _maxNumberOfPoints(1000000), _currNumberOfPoints(0),
      _pointsChanged(false) {
    _lines.resize(_maxNumberOfLines);
    _points.resize(_maxNumberOfPoints);
    // Line buffer
    {
        Mesh::CreateInfo info{};
        info.vertexBufferInfo.data = (uint8_t*)_lines.data();
        info.vertexBufferInfo.size = _lines.size() * sizeof(Line);
        info.vertexBufferInfo.layout.push(BufferLayout::Element::Type::VEC3, "iPos");
        info.vertexBufferInfo.layout.push(BufferLayout::Element::Type::VEC4, "iColor");
        _lineBuffer = gfx::create<gfx::Mesh>(info);
        LOG_DEBUG("Drawer", "Line buffer created");
    }

    // Point buffer
    {
        Mesh::CreateInfo info{};
        info.vertexBufferInfo.data = (uint8_t*)_points.data();
        info.vertexBufferInfo.size = _points.size() * sizeof(Point);
        info.vertexBufferInfo.layout.push(BufferLayout::Element::Type::VEC3, "iPos");
        info.vertexBufferInfo.layout.push(BufferLayout::Element::Type::VEC4, "iColor");
        _pointBuffer = gfx::create<gfx::Mesh>(info);
        LOG_DEBUG("Drawer", "Point buffer created");
    }
}

Drawer& Drawer::getInstance() {
    static Drawer drawer;
    return drawer;
}

void Drawer::clear(StringId group) { getInstance().clearImpl(group); }
void Drawer::clearImpl(StringId group) {
    if (group == "No group"_sid) {
        _currNumberOfLines = 0;
        _currNumberOfPoints = 0;
        _linesChanged = false;
        _pointsChanged = false;
    } else {
        clearImpl<Line>(group);
        clearImpl<Point>(group);
    }
}

} // namespace atta::graphics
