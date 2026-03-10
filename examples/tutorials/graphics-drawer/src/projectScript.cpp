//--------------------------------------------------
// Graphics Drawer
// projectScript.cpp
// Date: 2022-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"
#include <atta/graphics/drawer.h>

namespace gfx = atta::graphics;

void Project::onLoad() {
    // Vertical line
    gfx::Drawer::Line line;
    line.p0 = {0, 0, 0};
    line.p1 = {0, 0, 1};
    line.c0 = {1, 0, 0, 1};
    line.c1 = {0, 1, 0, 1};
    gfx::Drawer::add(line, "lines");

    // Square
    std::vector<atta::vec3> vertices = std::vector<atta::vec3>{{-0.9, -0.9, 0}, {0.9, -0.9, 0}, {0.9, 0.9, 0}, {-0.9, 0.9, 0}};
    std::vector<atta::vec4> colors = std::vector<atta::vec4>{{1, 1, 0, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 0, 1, 1}};
    for (int i = 0; i < vertices.size(); i++) {
        int i0 = i;
        int i1 = (i0 + 1) % vertices.size();
        line.p0 = vertices[i0];
        line.p1 = vertices[i1];
        line.c0 = colors[i0];
        line.c1 = colors[i1];
        gfx::Drawer::add(line, "square");
    }
}

void Project::onUpdateBefore(float dt) {
    static float time = 0;
    time += dt;

    gfx::Drawer::clear("circle");
    gfx::Drawer::Point point;
    gfx::Drawer::Line line;
    for (int i = 0; i < 100; i++) {
        float angle = i / float(100) * M_PI * 2;
        float c = cos(angle + time / 10);
        float s = sin(angle + time / 10);
        point.p = {c, s, 1};
        point.c = {(c + 1) / 2, (s + 1) / 2, 1, 1};
        gfx::Drawer::add(point, "circle");

        line.p0 = atta::vec3(atta::vec2(point.p) * 0.9f, 1.0f);
        line.p1 = {0, 0, 1};
        line.c0 = point.c;
        line.c1 = {0, 1, 0, 1};
        gfx::Drawer::add(line, "circle");
    }
}

void Project::onStop() { gfx::Drawer::clear("circle"); }

void Project::onUnload() {
    gfx::Drawer::clear("lines");
    gfx::Drawer::clear("square");
};
