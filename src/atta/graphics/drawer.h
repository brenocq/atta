//--------------------------------------------------
// Atta Graphics Module
// drawer.h
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_DRAWER_H
#define ATTA_GRAPHICS_DRAWER_H

#include <atta/graphics/mesh.h>
#include <atta/utils/math/math.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

// Used to draw 3d objects, mainly for debugging and visualization
class Drawer {
  public:
    struct Line {
        vec3 p0;
        vec4 c0 = vec4(1, 0, 1, 1);
        vec3 p1;
        vec4 c1 = vec4(1, 0, 1, 1);

        Line() = default;
        Line(vec3 p0_, vec3 p1_, vec4 c0_ = vec4(1, 0, 1, 1), vec4 c1_ = vec4(1, 0, 1, 1)) : p0(p0_), c0(c0_), p1(p1_), c1(c1_) {}
    };

    struct Point {
        vec3 p;
        vec4 c = vec4(1, 0, 1, 1);

        Point() = default;
        Point(vec3 p_, vec4 c_ = vec4(1, 0, 1, 1)) : p(p_), c(c_) {}
    };

    // Draw 3d objects
    template <typename T>
    static void add(T obj, StringId group = StringId("No group"));
    template <typename T>
    static void clear(StringId group = StringId("No group")); // Clear lines or points of specific group
    static void clear(StringId group = StringId("No group")); // Clear specific group or all groups

    // Get data
    template <typename T>
    static const std::vector<T>& get();
    template <typename T>
    static std::map<StringHash, std::vector<T>>& getGroups();
    template <typename T>
    static unsigned getMaxNumber();
    template <typename T>
    static unsigned getCurrNumber();

    // Core
    static Drawer& getInstance();

  private:
    Drawer();

    // Draw 3d objects implementation
    template <typename T>
    void addImpl(T obj, StringId group);
    template <typename T>
    void clearImpl(StringId group);
    void clearImpl(StringId group);

    // Get data implementation
    template <typename T>
    const std::vector<T>& getImpl();
    template <typename T>
    std::map<StringHash, std::vector<T>>& getGroupsImpl();
    template <typename T>
    unsigned getMaxNumberImpl();
    template <typename T>
    unsigned getCurrNumberImpl();
    template <typename T>
    unsigned getChanged();
    template <typename T>
    void setCurrNumber(unsigned number);
    template <typename T>
    void setChanged(bool changed);

    // The _lines vector is updated only when getImpl()
    // is called and _linesChanged is true. Analogous to _points
    std::map<StringHash, std::vector<Line>> _lineGroups;
    unsigned _maxNumberOfLines;
    unsigned _currNumberOfLines; // Always have the right number of lines, even if _lines still need to be updated
    bool _linesChanged;
    std::vector<Line> _lines; // Updated only when get() is called

    std::map<StringHash, std::vector<Point>> _pointGroups;
    unsigned _maxNumberOfPoints;
    unsigned _currNumberOfPoints; // Always have the right number of points, even if _points still need to be updated
    bool _pointsChanged;
    std::vector<Point> _points; // Updated only when get() is called

    std::shared_ptr<Mesh> _lineBuffer;  ///< Mesh with every two vertices being a line
    std::shared_ptr<Mesh> _pointBuffer; ///< Mesh with every vertex being a point
};
} // namespace atta::graphics

#include <atta/graphics/drawer.inl>

#endif // ATTA_GRAPHICS_DRAWER_H
