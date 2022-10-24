//--------------------------------------------------
// Atta UI Module
// toolBar.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/events/simulationContinue.h>
#include <atta/event/events/simulationPause.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStep.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/interface.h>
#include <atta/graphics/interface.h>
#include <atta/ui/layers/editor/toolBar/toolBar.h>
#include <atta/ui/layers/editor/components/button.h>
#include <atta/utils/config.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace atta::ui {

void ToolBar::render() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 1.0f));

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&window_class);

    ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        float buttonH = ImGui::GetWindowHeight() - 10.0f;
        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f -
                             (4 * buttonH + 150 + ImGui::CalcTextSize("00:00:00.000 - 0.000x").x + 5 * 10 + 10) / 2);

        // View button
        {
            if (renderButton(graphics::getViewportRendering() ? "view" : "no-view", buttonH))
                graphics::setViewportRendering(!graphics::getViewportRendering());
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(10.0f, 0.0f));
        }

        // Step buttons
        {
            ImGui::SameLine();
            bool disabled = Config::getState() == Config::State::IDLE;
            if (disabled) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            if (renderButton("stop", buttonH)) {
                event::SimulationStop e;
                event::publish(e);
            }
            if (disabled) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }

            ImGui::SameLine();
            if (Config::getState() != Config::State::RUNNING) {
                if (renderButton("play", buttonH)) {
                    if (Config::getState() == Config::State::PAUSED) {
                        event::SimulationContinue e;
                        event::publish(e);
                    } else if (Config::getState() == Config::State::IDLE) {
                        event::SimulationStart e;
                        event::publish(e);
                    }
                }
            } else {
                if (renderButton("pause", buttonH)) {
                    event::SimulationPause e;
                    event::publish(e);
                }
            }

            ImGui::SameLine();
            if (renderButton("step", buttonH)) {
                event::SimulationStep e;
                event::publish(e);
            }
        }

        // Slider
        {
            static bool firstTime = true;
            static float speed = 1.0f;
            // Real time interval
            const float rtInt = 0.1f;

            if(firstTime)
            {
                firstTime = false;

                // Initialize speed slider with correct value
                // Inverse function from speed to desiredStepSpeed
                float desiredStepSpeed = Config::getDesiredStepSpeed();
                if (desiredStepSpeed == Config::getDt())
                    speed = 0.0f;
                else if (desiredStepSpeed == 1.0f)
                    speed = 1.0f;
                else if (desiredStepSpeed == 0.0f)
                    speed = 2.0f;
                else if (desiredStepSpeed < 1.0f) {
                    if(Config::getDt() != 1)
                    {
                        float k = (desiredStepSpeed-Config::getDt())/(1-Config::getDt());
                        speed = log10(9*k+1)-rtInt;
                    }
                } else
                    speed = log10(desiredStepSpeed)/2+rtInt+1;
            }

            // Slider text
            std::string text = "Real time";
            if (speed < 1.0f && speed > 0.5f)
                text = "Slow";
            else if (speed <= 0.5f && speed > 0.0f)
                text = "Very slow";
            else if (speed <= 0.0f)
                text = "Stepping";
            else if (speed > 1.0f && speed < 1.5f)
                text = "Fast";
            else if (speed >= 1.5f && speed < 2.0f)
                text = "Very fast";
            else if (speed >= 2.0f)
                text = "Maximum speed";

            // Render slider
            ImGui::PushItemWidth(150);
            ImGui::SameLine();
            if (ImGui::SliderFloat("##DesiredSpeedSlider", &speed, 0.0f, 2.0f, text.c_str())) {
                if (speed > 1.0 - rtInt && speed < 1.0 + rtInt)
                    speed = 1.0f;

                // Convert to desired speed
                if (speed == 0.0f)
                    Config::setDesiredStepSpeed(Config::getDt()); // One step per second
                else if (speed == 1.0f)
                    Config::setDesiredStepSpeed(1.0f); // Real time
                else if (speed == 2.0f)
                    Config::setDesiredStepSpeed(0.0f); // As fast as possible
                else if (speed < 1.0f) {
                    float k = (pow(10.0f, speed + rtInt) - 1) / 9.0f;
                    Config::setDesiredStepSpeed(1.0f * k + Config::getDt() * (1 - k)); // Slower than real time
                } else {
                    float speedPow = pow(10.0f, (speed - rtInt - 1.0f) * 2); // [1,2] -> [1,100]
                    Config::setDesiredStepSpeed(speedPow);                   // Faster than real time
                }
            }

            // Time text
            float time = Config::getTime();
            int h = time / 3600;
            int m = (time - h * 3600) / 60;
            int s = (time - h * 3600 - m * 60);
            int ms = (time - int(time)) * 1000;
            ImGui::SameLine();
            ImGui::Text("%02d:%02d:%02d.%03d - %.3fx", h, m, s, ms, Config::getRealStepSpeed());
        }
    }
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(1);
    ImGui::End();
}

} // namespace atta::ui
