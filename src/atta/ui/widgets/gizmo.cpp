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
void Gizmo::setCamera(std::weak_ptr<gfx::Camera> camera) { _camera = camera; }
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
    std::shared_ptr<gfx::Camera> camera = _camera.lock();
    cmp::Transform* t = cmp::getComponent<cmp::Transform>(entity);
    if (camera && t) {
        mat4 transform = transpose(t->getWorldTransformMatrix(entity));

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x - 10.0f, windowSize.y - 8.0f);

        ImGuizmo::OPERATION operation = convert(_operation);
        ImGuizmo::MODE mode = convert(_mode);

        ImGuizmo::SetOrthographic(camera->getName() == "OrthographicCamera");
        mat4 view = transpose(camera->getView());
        mat4 proj = transpose(camera->getProj());
        proj.mat[1][1] *= -1;

        float snapValue = 0.5f;
        if (operation == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;
        float snapValues[3] = {snapValue, snapValue, snapValue};

        if (ImGuizmo::Manipulate(view.data, proj.data, operation, mode, transform.data, nullptr, _snap ? snapValues : nullptr)) {
            transform.transpose();

            // Get changed
            vec3 pos, scale;
            quat newOri;
            transform.getPosOriScale(pos, newOri, scale);
            vec3 oriDelta = newOri.getEuler() - t->orientation.getEuler();
            quat ori;
            ori.setEuler(t->orientation.getEuler() + oriDelta);

            // Delta world to local
            cmp::Relationship* r = cmp::getComponent<cmp::Relationship>(entity);
            if (r && r->getParent() != -1) {
                // Get transform of the first entity that has transform when going up in the hierarchy
                cmp::Transform* pt = nullptr;
                cmp::EntityId parentId = -1;
                while (pt == nullptr) {
                    parentId = r->getParent();
                    pt = cmp::getComponent<cmp::Transform>(parentId);
                    r = cmp::getComponent<cmp::Relationship>(parentId);
                    if (r->getParent() == -1)
                        break;
                }

                // If found some entity with transform component, convert result to be relative to it
                if (pt) {
                    cmp::Transform pTransform = pt->getWorldTransform(parentId);
                    vec3 pPos = pTransform.position;
                    vec3 pScale = pTransform.scale;
                    quat pOri = pTransform.orientation;

                    // Calculate pos ori scale relative to parent
                    pos -= pPos;
                    scale /= pScale;
                    ori = ori * (-pOri); // Rotation from pOri to ori
                }
            }

            // Update entity transform
            if (operation == ImGuizmo::OPERATION::TRANSLATE)
                t->position = pos;
            else if (operation == ImGuizmo::OPERATION::ROTATE)
                t->orientation = ori;
            else if (operation == ImGuizmo::OPERATION::SCALE)
                t->scale = scale;

            // cmp::RigidBody2D* rb2d = cmp::getComponent<cmp::RigidBody2D>(entity);
            // if (rb2d) {
            //     if (mouseOperation == ImGuizmo::OPERATION::TRANSLATE || mouseOperation == ImGuizmo::OPERATION::ROTATE) {
            //         vec2 pos = vec2(t->position);
            //         float angle = -t->orientation.getEuler().z;
            //         rb2d->setTransform(pos, angle);
            //     } else if (mouseOperation == ImGuizmo::OPERATION::SCALE) {
            //         // TODO Recreate box2d rigid body
            //     }
            // }
            return true;
        }
    }
    return false;
}

} // namespace atta::ui
