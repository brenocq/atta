//--------------------------------------------------
// Atta UI System
// toolBar.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/toolBar/toolBar.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/simulationStartEvent.h>
#include <atta/eventSystem/events/simulationContinueEvent.h>
#include <atta/eventSystem/events/simulationPauseEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace atta::ui
{
    ToolBar::ToolBar():
        _editorState(EditorState::EDITOR)
    {
        EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
        EventManager::subscribe<SimulationContinueEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
        EventManager::subscribe<SimulationPauseEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
        EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
    }

    void ToolBar::render()
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

    void ToolBar::onSimulationStateChange(Event& event)
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
