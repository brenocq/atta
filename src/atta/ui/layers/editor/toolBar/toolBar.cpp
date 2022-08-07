//--------------------------------------------------
// Atta UI Module
// toolBar.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/core/config.h>
#include <atta/event/eventManager.h>
#include <atta/event/events/simulationContinueEvent.h>
#include <atta/event/events/simulationPauseEvent.h>
#include <atta/event/events/simulationStartEvent.h>
#include <atta/event/events/simulationStopEvent.h>
#include <atta/graphics/graphicsManager.h>
#include <atta/ui/layers/editor/toolBar/toolBar.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace atta::ui {
ToolBar::ToolBar() : _editorState(EditorState::EDITOR) {
    EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
    EventManager::subscribe<SimulationContinueEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
    EventManager::subscribe<SimulationPauseEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
    EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(ToolBar::onSimulationStateChange));
}

void ToolBar::render() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    // const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 1.0f));

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&window_class);

    ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        float buttonH = ImGui::GetWindowHeight() - 10.0f;

        switch (_editorState) {
        case EditorState::EDITOR: {
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f));
            if (ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/play.png"_ssid), ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1), 0)) {
                SimulationStartEvent e;
                EventManager::publish(e);
                _editorState = EditorState::SIMULATION_RUNNING;
            }
            break;
        }
        case EditorState::SIMULATION_RUNNING:
        case EditorState::SIMULATION_PAUSED: {
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - buttonH / 2 - 2.0f);
            if (_editorState == EditorState::SIMULATION_RUNNING)
                if (ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/pause.png"_ssid), ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1),
                                       0)) {
                    SimulationPauseEvent e;
                    EventManager::publish(e);
                    _editorState = EditorState::SIMULATION_PAUSED;
                }
            if (_editorState == EditorState::SIMULATION_PAUSED)
                if (ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/play.png"_ssid), ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1),
                                       0)) {
                    SimulationContinueEvent e;
                    EventManager::publish(e);
                    _editorState = EditorState::SIMULATION_RUNNING;
                }

            ImGui::SameLine();
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) + buttonH / 2 + 2.0f);
            if (ImGui::ImageButton(GraphicsManager::getImGuiImage("icons/stop.png"_ssid), ImVec2(buttonH, buttonH), ImVec2(0, 0), ImVec2(1, 1), 0)) {
                SimulationStopEvent e;
                EventManager::publish(e);
                _editorState = EditorState::EDITOR;
            }
            break;
        }
        default: {
            LOG_WARN("EditorLayer", "Invalid editor state: [w]$0[]", static_cast<int>(_editorState));
        }
        }
        ImGui::SameLine();

        float dt = Config::getDt();
        float step = 1.0f;
        if (dt > 0.0f)
            while (step > dt)
                step /= 10.0f;
        step /= 10;

        int width = 100;
        int padding = 30;
        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - width - padding);
        ImGui::SetCursorPosY(7);
        ImGui::PushItemWidth(width);
        ImGui::DragFloat("dt", &dt, step, 0.000001f, 1.0f, "%.6f");
        float dtClean = int(dt / step) * step; // Ex: 0.001234f -> 0.001200f
        Config::setDt(dtClean > 0.0f ? dtClean : dt);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(1);
    ImGui::End();

    handleShortcuts();
}

void ToolBar::onSimulationStateChange(Event& event) {
    switch (event.getType()) {
    case SimulationStartEvent::type: {
        _editorState = EditorState::SIMULATION_RUNNING;
        break;
    }
    case SimulationContinueEvent::type: {
        _editorState = EditorState::SIMULATION_RUNNING;
        break;
    }
    case SimulationPauseEvent::type: {
        _editorState = EditorState::SIMULATION_PAUSED;
        break;
    }
    case SimulationStopEvent::type: {
        _editorState = EditorState::EDITOR;
        break;
    }
    default: {
        LOG_WARN("EditorLayer", "Unknown simulation event");
    }
    }
}

void ToolBar::handleShortcuts() {
    bool notMoving = !ImGui::IsMouseDown(ImGuiMouseButton_Middle);

    // Play/pause shortcut
    if (ImGui::IsKeyPressed('P') && notMoving) {
        switch (_editorState) {
        case EditorState::EDITOR:
        case EditorState::SIMULATION_PAUSED:
            changeState(EditorState::SIMULATION_RUNNING);
            break;
        case EditorState::SIMULATION_RUNNING:
            changeState(EditorState::SIMULATION_PAUSED);
            break;
        }
    }

    // Stop shortcut
    if (ImGui::IsKeyPressed('S') && notMoving)
        changeState(EditorState::EDITOR);
}

void ToolBar::changeState(ToolBar::EditorState newState) {
    switch (newState) {
    case EditorState::EDITOR: {
        if (_editorState != EditorState::EDITOR) {
            SimulationStopEvent e;
            EventManager::publish(e);
        }
        _editorState = EditorState::EDITOR;
        break;
    }
    case EditorState::SIMULATION_RUNNING: {
        if (_editorState == EditorState::SIMULATION_PAUSED) {
            SimulationContinueEvent e;
            EventManager::publish(e);
        } else if (_editorState == EditorState::EDITOR) {
            SimulationStartEvent e;
            EventManager::publish(e);
        }
        _editorState = EditorState::SIMULATION_RUNNING;
        break;
    }
    case EditorState::SIMULATION_PAUSED: {
        // If it is not running, need to start the simulation
        if (_editorState == EditorState::EDITOR) {
            SimulationStartEvent e;
            EventManager::publish(e);
            _editorState = EditorState::SIMULATION_RUNNING;
        }

        // After the simulation is started, it is possible to pause
        if (_editorState == EditorState::SIMULATION_RUNNING) {
            SimulationPauseEvent e;
            EventManager::publish(e);
            _editorState = EditorState::SIMULATION_PAUSED;
        }
        break;
    }
    default:
        LOG_WARN("ToolBar", "Try to change editor state to an invalid one");
    }
}
} // namespace atta::ui
