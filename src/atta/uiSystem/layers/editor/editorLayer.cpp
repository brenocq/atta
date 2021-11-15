//--------------------------------------------------
// Atta UI System
// editorLayer.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/editorLayer.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/texture.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/simulationStartEvent.h>
#include <atta/eventSystem/events/simulationContinueEvent.h>
#include <atta/eventSystem/events/simulationPauseEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/scriptSystem/scriptManager.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindow.h>

namespace atta::ui
{
    EditorLayer::EditorLayer():
        Layer(StringId("GraphicsEditorLayer"))
    {
    }

    void EditorLayer::onAttach()
    {
        EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(EditorLayer::onSimulationStateChange));
        EventManager::subscribe<SimulationContinueEvent>(BIND_EVENT_FUNC(EditorLayer::onSimulationStateChange));
        EventManager::subscribe<SimulationPauseEvent>(BIND_EVENT_FUNC(EditorLayer::onSimulationStateChange));
        EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(EditorLayer::onSimulationStateChange));

        _editorState = EditorState::EDITOR;
    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onRender()
    {
    }

    void EditorLayer::onUIRender()
    {
        _dockSpace.render();
        toolbar();

        //if(_editorState != EditorState::SIMULATION_RUNNING)
        {
            _topBar.render();
            //bool demo = true;
            //ImGui::ShowDemoWindow(&demo);

            _entityWindow.render();
            IOSystemWindow::render();

            ProjectScript* project = ScriptManager::getProjectScript();
            if(project)
                project->onUIRender();

            ImGui::Begin("Debug");
            ImGui::Text("Hello, down!");
            ImGui::End();
        }
        updateViewports();
    }

    void EditorLayer::updateViewports()
    {
        std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
        static int activeViewport = 0;

        int i = -1;
        for(auto viewport : viewports)
        {
            ImGui::DockBuilderDockWindow((viewport->getName()+"###Viewport"+viewport->getSID().getString()).c_str(), _dockSpace.getViewportDockId());
            char nameBuf[128];
            sprintf(nameBuf, "%s###Viewport%s", viewport->getName().c_str(), viewport->getSID().getString().c_str());
            i++;
            // Render and resize
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
            bool open = true;
            ImGui::Begin(nameBuf, &open);
            {
                //----- Move camera -----//
                // Check started camera movement
                if(ImGui::IsMouseClicked(2) && ImGui::IsWindowHovered())
                    activeViewport = i;

                // Update camera (wheel pressed)
                if(activeViewport==i && ImGui::IsMouseDown(2))
                    viewport->getCamera()->move();

                //----- Mouse click -----//
                vec2i click = {-1, -1};
                if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
                {
                    ImVec2 window = ImGui::GetWindowPos();
                    ImVec2 cursor = ImGui::GetCursorPos();
                    ImVec2 mouse = ImGui::GetMousePos();
                    click = { int(mouse.x-window.x-cursor.x), int(mouse.y-window.y-cursor.y) };
                }

                static ImGuizmo::OPERATION mouseMode = ImGuizmo::OPERATION::TRANSLATE;

                if(ImGui::IsWindowHovered())
                {
                    ImGuiIO& io = ImGui::GetIO();
                    if(ImGui::IsKeyPressed('T') && io.KeyCtrl)
                        mouseMode = ImGuizmo::OPERATION::TRANSLATE;
                    else if(ImGui::IsKeyPressed('S') && io.KeyCtrl)
                        mouseMode = ImGuizmo::OPERATION::SCALE;
                    else if(ImGui::IsKeyPressed('R') && io.KeyCtrl)
                        mouseMode = ImGuizmo::OPERATION::ROTATE;
                }

                //----- Render to texture -----//
                ImVec2 size = ImVec2(viewport->getWidth(), viewport->getHeight());
                ImGui::Image(viewport->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

                //----- ImGuizmo -----//
                bool imGuizmoUsingMouse = false;
                EntityId entity = ComponentManager::getSelectedEntity();
                if(entity >= 0)
                {
                    TransformComponent* t = ComponentManager::getEntityComponent<TransformComponent>(entity);
                    if(t)
                    {
                        ImGuizmo::SetOrthographic(false);
                        ImGuizmo::SetDrawlist();
                        ImGuizmo::SetRect(ImGui::GetWindowPos().x+5.0f, ImGui::GetWindowPos().y+25.0f, viewport->getWidth(), viewport->getHeight());
                        mat4 view = transpose(viewport->getCamera()->getView());
                        mat4 proj = viewport->getCamera()->getProj();
                        proj.mat[1][1] *= -1;
                        proj.transpose();

                        mat4 transform = t->getTransform();
                        transform.transpose();
                        ImGuizmo::Manipulate(view.data, proj.data, mouseMode, ImGuizmo::LOCAL, transform.data);
                        if(ImGuizmo::IsUsing())
                        {
                            imGuizmoUsingMouse = true;
                            transform.transpose();
                            transform.getPosOriScale(t->position, t->orientation, t->scale);
                        }
                    }
                }

                //----- Mouse click selection -----//
                if(!imGuizmoUsingMouse)
                {
                    if(click.x >= 0 && click.y >=0 && click.x < (int)viewport->getWidth() && click.y < (int)viewport->getHeight())
                    {
                        EntityId eid = GraphicsManager::viewportEntityClick(viewport, click);
                        ComponentManager::setSelectedEntity(eid);
                    }
                }

                //----- Overlay -----//
                {
                    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
                    ImVec2 window = ImGui::GetWindowPos();
                    ImGui::SetNextWindowPos(ImVec2(window.x+10, window.y+30));
                    ImGui::SetNextWindowBgAlpha(0.35f);
                    bool open = true;
                    if(ImGui::Begin((viewport->getSID().getString()+"Overlay").c_str(), &open, window_flags))
                    {
                        ImGui::Text("To move the camera");
                        ImGui::BulletText("Holding mouse middle button");
                        ImGui::BulletText("Rotate with mouse");
                        ImGui::BulletText("Move with ASWD QE");
                        ImGui::Text("To move objects");
                        ImGui::BulletText("Select some object");
                        ImGui::BulletText("Translate: CTRL+t");
                        ImGui::BulletText("Scale: CTRL+s");
                        ImGui::BulletText("Rotate: CTRL+r");
                    }
                    ImGui::End();
                }

                //----- Resize -----//
                ImVec2 windowSize = ImGui::GetWindowSize();
                if(windowSize.x != size.x || windowSize.y != size.y)
                    viewport->resize((uint32_t)windowSize.x-10, (uint32_t)windowSize.y-30);
            }
            ImGui::End();
            ImGui::PopStyleVar(1);

            if(!open)
                GraphicsManager::removeViewport(viewport);
        }
    }

    void EditorLayer::toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        //const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 1.0f));

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
        ImGui::SetNextWindowClass(&window_class);

        ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            float buttonH = ImGui::GetWindowHeight() - 10.0f;

            switch(_editorState)
            {
            case EditorState::EDITOR:
                {
                    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f));
                    if(ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/play.png"_ssid), 
                                ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1), 0))
                    {
                        SimulationStartEvent e;
                        EventManager::publish(e);
                        _editorState = EditorState::SIMULATION_RUNNING;
                    }
                    break;
                }
            case EditorState::SIMULATION_RUNNING:
            case EditorState::SIMULATION_PAUSED:
                {
                    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f)-buttonH/2-2.0f);
                    if(_editorState == EditorState::SIMULATION_RUNNING)
                        if(ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/pause.png"_ssid), 
                                    ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1), 0))
                        {
                            SimulationPauseEvent e;
                            EventManager::publish(e);
                            _editorState = EditorState::SIMULATION_PAUSED;
                        }
                    if(_editorState == EditorState::SIMULATION_PAUSED)
                        if(ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/play.png"_ssid), 
                                    ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1), 0))
                        {
                            SimulationContinueEvent e;
                            EventManager::publish(e);
                            _editorState = EditorState::SIMULATION_RUNNING;
                        }

                    ImGui::SameLine();
                    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f)+buttonH/2+2.0f);
                    if(ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/stop.png"_ssid), ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1), 0))
                    {
                        SimulationStopEvent e;
                        EventManager::publish(e);
                        _editorState = EditorState::EDITOR;
                    }
                    break;
                }
            default:
                {
                    LOG_WARN("EditorLayer", "Invalid editor state: [w]$0[]", static_cast<int>(_editorState));
                }
            }
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(1);
        ImGui::End();
    }

    void EditorLayer::onSimulationStateChange(Event& event)
    {
        switch(event.getType())
        {
        case SimulationStartEvent::type:
            {
                _editorState = EditorState::SIMULATION_RUNNING;
                break;
            }
        case SimulationContinueEvent::type:
            {
                _editorState = EditorState::SIMULATION_RUNNING;
                break;
            }
        case SimulationPauseEvent::type:
            {
                _editorState = EditorState::SIMULATION_PAUSED;
                break;
            }
        case SimulationStopEvent::type:
            {
                _editorState = EditorState::EDITOR;
                break;
            }
        default:
            {
                LOG_WARN("EditorLayer", "Unknown simulation event");
            }
        }
    }
}
