//--------------------------------------------------
// Atta UI Module
// guizmo.h
// Date: 2024-01-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/widgets/gizmo.h>

#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <ImGuizmo.h>

namespace atta::ui {

Gizmo::Gizmo() : _operation(TRANSLATE), _mode(WORLD), _snap(false) {}

void Gizmo::setOperation(Operation operation) { _operation = operation; }
void Gizmo::setMode(Mode mode) { _mode = mode; }
void Gizmo::setViewport(std::weak_ptr<Viewport> viewport) { _viewport = viewport; }
void Gizmo::setSnap(bool snap) { _snap = snap; }

ImGuizmo::OPERATION convert(Gizmo::Operation operation) {
    ImGuizmo::OPERATION result{};
    if ((operation & Gizmo::TRANSLATE) > 0)
        result = result | ImGuizmo::OPERATION::TRANSLATE;
    if ((operation & Gizmo::ROTATE) > 0)
        result = result | ImGuizmo::OPERATION::ROTATE;
    if ((operation & Gizmo::SCALE) > 0)
        result = result | ImGuizmo::OPERATION::SCALE;
    return result;
}

ImGuizmo::MODE convert(Gizmo::Mode mode) { return mode == Gizmo::WORLD ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL; }

bool Gizmo::manipulate(cmp::EntityId entity) {
    std::shared_ptr<Viewport> viewport = _viewport.lock();
    cmp::Transform* t = cmp::getComponent<cmp::Transform>(entity);
    if (viewport && t) {
        mat4 transform = transpose(t->getWorldTransformMatrix(entity));

        ImGuizmo::SetOrthographic(viewport->getCamera()->getName() == "OrthographicCamera");
        ImGuizmo::SetDrawlist();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImGuizmo::SetRect(windowPos.x, windowPos.y + 22.0f, viewport->getWidth(), viewport->getHeight());

        ImGuizmo::OPERATION operation = convert(_operation);
        ImGuizmo::MODE mode = convert(_mode);

        mat4 view = transpose(viewport->getCamera()->getView());
        mat4 proj = transpose(viewport->getCamera()->getProj());
        proj.mat[1][1] *= -1;

        float snapValue = 0.5f;
        if (operation == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;
        float snapValues[3] = {snapValue, snapValue, snapValue};

        ImGuizmo::Manipulate(view.data, proj.data, operation, mode, transform.data, nullptr, _snap ? snapValues : nullptr);
        if (ImGuizmo::IsUsing()) {
            transform.transpose();

            // Set new world transform
            cmp::Transform newT;
            transform.getPosOriScale(newT.position, newT.orientation, newT.scale);
            t->setWorldTransform(entity, newT);
            return true;
        }
    }
    return false;
}

} // namespace atta::ui
