//--------------------------------------------------
// Atta UI Module
// guizmo.h
// Date: 2024-01-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/editor/widgets/gizmo.h>

#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <ImGuizmo.h>

namespace atta::ui {

Gizmo::Gizmo() : _operation(TRANSLATE), _mode(WORLD), _isOrthographic(false), _snap(false) {}

void Gizmo::setOperation(Operation operation) { _operation = operation; }
void Gizmo::setMode(Mode mode) { _mode = mode; }
void Gizmo::setSnap(bool snap) { _snap = snap; }

void Gizmo::setCamera(std::weak_ptr<Camera> camera) { _camera = camera; }

ImGuizmo::OPERATION convert(Operation operation) {
    ImGuizmo::OPERATION result{};
    if (operation & TRANSLATE > 0)
        result = result | ImGuizmo::OPERATION::TRANSLATE;
    if (operation & ROTATE > 0)
        result = result | ImGuizmo::OPERATION::ROTATE;
    if (operation & SCALE > 0)
        result = result | ImGuizmo::OPERATION::SCALE;
    return result;
}

ImGuizmo::MODE convert(Mode mode) { return mode == WORLD ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL; }

bool Gizmo::manipulate(component::EntityId entity) {
    std::shared_ptr<Camera> camera = _camera.lock();
    component::Transform* t = component::getComponent<component::Transform>(entity);
    if (t) {
        mat4 transform = transpose(t->getWorldTransformMatrix(entity));

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x + 5.0f, ImGui::GetWindowPos().y + 24.0f, 500, 500);

        ImGuizmo::OPERATION operation = convert(_operation);
        ImGuizmo::MODE mode = convert(_mode);

        ImGuizmo::SetOrthographic(camera->getName() == "OrthographicCamera");
        mat4 view = transpose(camera->getView());
        mat4 proj = transpose(camera->getProj());
        proj.mat[1][1] *= -1;

        if (ImGuizmo::Manipulate(transpose(_view).data, _proj.data, operation, mode, transform.data, nullptr, snap ? snapValues : nullptr)) {
            transform.transpose();

            // Get changed
            vec3 pos, scale;
            quat newOri;
            transform.getPosOriScale(pos, newOri, scale);
            vec3 oriDelta = newOri.getEuler() - t->orientation.getEuler();
            quat ori;
            ori.setEuler(t->orientation.getEuler() + oriDelta);

            // Delta world to local
            component::Relationship* r = component::getComponent<component::Relationship>(entity);
            if (r && r->getParent() != -1) {
                // Get transform of the first entity that has transform when going up in the hierarchy
                component::Transform* pt = nullptr;
                component::EntityId parentId = -1;
                while (pt == nullptr) {
                    parentId = r->getParent();
                    pt = component::getComponent<component::Transform>(parentId);
                    r = component::getComponent<component::Relationship>(parentId);
                    if (r->getParent() == -1)
                        break;
                }

                // If found some entity with transform component, convert result to be relative to it
                if (pt) {
                    component::Transform pTransform = pt->getWorldTransform(parentId);
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
            if (mouseOperation == ImGuizmo::OPERATION::TRANSLATE)
                t->position = pos;
            else if (mouseOperation == ImGuizmo::OPERATION::ROTATE)
                t->orientation = ori;
            else if (mouseOperation == ImGuizmo::OPERATION::SCALE)
                t->scale = scale;

            // component::RigidBody2D* rb2d = component::getComponent<component::RigidBody2D>(entity);
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
