//--------------------------------------------------
// Atta Graphics Module
// drawer.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/drawer.h>
#include <atta/resource/interface.h>

namespace atta::graphics {

StringId Drawer::lineMeshName = "atta::gfx::Drawer::line";
StringId Drawer::pointMeshName = "atta::gfx::Drawer::point";

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
        res::create<res::Mesh>(lineMeshName, info);
    }

    // Point buffer
    {
        res::Mesh::CreateInfo info{};
        uint8_t* data = (uint8_t*)_points.data();
        size_t size = _points.size() * sizeof(Point);
        info.vertices = std::vector<uint8_t>(data, data + size);
        info.vertexLayout.push_back({resource::Mesh::VertexElement::VEC3, "iPos"});
        info.vertexLayout.push_back({resource::Mesh::VertexElement::VEC4, "iColor"});
        res::create<res::Mesh>(pointMeshName, info);
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

void Drawer::update() {
    getInstance().updateImpl<Drawer::Line>();
    getInstance().updateImpl<Drawer::Point>();
}

template <typename T>
void Drawer::updateImpl() {
    if constexpr (std::is_same<T, Drawer::Line>::value || std::is_same<T, Drawer::Point>::value) {
        if (getChanged<T>()) {
            setCurrNumber<T>(0);

            // Populate vertex buffer
            std::map<StringHash, std::vector<T>> groups = getGroupsImpl<T>();
            for (auto& [key, group] : groups) {
                // Copy group objects to preallocated vector
                for (uint32_t i = 0; i < group.size(); i++) {
                    if constexpr (std::is_same<T, Drawer::Line>::value)
                        _lines[_currNumberOfLines++] = group[i];
                    else if constexpr (std::is_same<T, Drawer::Point>::value)
                        _points[_currNumberOfPoints++] = group[i];
                }
            }

            // Update mesh
            if constexpr (std::is_same<T, Drawer::Line>::value) {
                res::Mesh* lineMesh = res::get<res::Mesh>(lineMeshName);
                uint8_t* data = (uint8_t*)_lines.data();
                size_t size = _currNumberOfLines * sizeof(Line);
                lineMesh->updateVertices(std::vector<uint8_t>(data, data + size));
            } else if constexpr (std::is_same<T, Drawer::Point>::value) {
                res::Mesh* pointMesh = res::get<res::Mesh>(pointMeshName);
                uint8_t* data = (uint8_t*)_points.data();
                size_t size = _currNumberOfPoints * sizeof(Point);
                pointMesh->updateVertices(std::vector<uint8_t>(data, data + size));
            }

            // Set changed as false
            setChanged<T>(false);
        }
    }
}

} // namespace atta::graphics
