//--------------------------------------------------
// Atta UI System
// topBar.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/topBar/topBar.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/fileSystem/fileManager.h>
#include <imgui_internal.h>

#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/renderers/phongRenderer.h>
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>

#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindow.h>

namespace atta::ui
{
    TopBar::TopBar():
        _showPreferences(false)
    {

    }

    void TopBar::render()
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {

                if(FileManager::isProjectOpen())
                {
                    ImGui::Text(FileManager::getProject()->getName().c_str());
                    ImGui::Separator();

                    if(ImGui::MenuItem("Close"))
                    {
                        FileManager::saveProject();
                        FileManager::closeProject();
                        ComponentManager::clear();
                        ComponentManager::createDefault();

                        // Replace viewports with default
                        GraphicsManager::createDefaultViewports();
                    }
                }

                if(ImGui::MenuItem("Open"))
                    _showOpenProject = true;

                if(FileManager::isProjectOpen())
                    if(ImGui::MenuItem("Save"))
                        FileManager::saveProject();

                if(ImGui::MenuItem("Save as"))
                    _showCreateProject = true;

                ImGui::Separator();

                if(ImGui::MenuItem("Quit"))
                {
                    WindowCloseEvent e;
                    EventManager::publish(e);
                }
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Edit"))
            {
                if(ImGui::MenuItem("Preferences"))
                    _showPreferences = true;

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Viewports"))
            {
                std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
                _viewportModals.resize(viewports.size());
                int i = 0;
                for(auto viewport : viewports)
                {
                    if(ImGui::MenuItem(viewport->getSID().getString().c_str()))
                        _viewportModals[i] = true;
                    i++;
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Create viewport"))
                {
                    // Choose viewport name
                    unsigned newViewportNumber = 0;
                    bool found = false;
                    while(!found)
                    {
                        found = true;
                        for(auto viewport : viewports) 
                            if(viewport->getSID() == StringId("Viewport "+std::to_string(newViewportNumber)))
                            {
                                found = false;
                                break;
                            }
                        if(!found)
                            newViewportNumber++;
                    }

                    // Create viewport
                    Viewport::CreateInfo viewportInfo;
                    viewportInfo.renderer = std::make_shared<PhongRenderer>();
                    viewportInfo.camera = std::static_pointer_cast<Camera>(std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{}));
                    viewportInfo.sid = StringId("Viewport "+std::to_string(newViewportNumber));
                    std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(viewportInfo);
                    GraphicsManager::addViewport(viewport);
                    ImGui::DockBuilderDockWindow((viewportInfo.sid.getString()+"###Viewport"+viewportInfo.sid.getString()).c_str(), _viewportDockId);
                    //ImGui::DockBuilderDockWindow("Viewport 0###ViewportViewport 0", _viewportDockId);
                }

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Systems"))
            {
                if(ImGui::MenuItem("IO System"))
                    IOSystemWindow::setOpen(true);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
        openProjectModal();
        createProjectModal();
        preferences();
        viewportModals();
    }

    void TopBar::preferences()
    {
        if(_showPreferences)
        {
            if(ImGui::Begin("Preferences", &_showPreferences))
            {
                std::vector<std::string> stringIds = StringId::getStrings();
                for(const auto& str : stringIds)
                    ImGui::Text(str.c_str());
            }
            ImGui::End();
        }
    }

    void TopBar::openProjectModal()
    {
        static bool lastShow = false;
        if(_showOpenProject && !lastShow)
        {
            // OBS: Doing this because can't open popup inside menuitem
            ImGui::OpenPopup("Open Project##OpenProjectModal");
            lastShow = _showOpenProject;
        }

        if(ImGui::BeginPopupModal("Open Project##OpenProjectModal"))
        {
            ImGui::Text("For now, you need to paste the absolute location for the .atta file below");
            ImGui::Separator();

            static char buf[254] = "";
            ImGui::InputText("##openProjectAttaPath", buf, sizeof(buf));

            if(ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
                _showOpenProject = false;
                lastShow = false;
            }

            ImGui::SameLine();

            if(ImGui::Button("Open"))
            { 
                FileManager::saveProject();
                FileManager::openProject(fs::path(buf));
                ImGui::CloseCurrentPopup();
                _showOpenProject = false;
                lastShow = false;
            }
            ImGui::SetItemDefaultFocus();

            ImGui::EndPopup();
        }
    }

    void TopBar::createProjectModal()
    {
        static bool lastShow = false;
        if(_showCreateProject && !lastShow)
        {
            // OBS: Doing this because can't open popup inside menuitem
            ImGui::OpenPopup("Create Project##CreateProjectModal");
            lastShow = _showCreateProject;
        }

        if(ImGui::BeginPopupModal("Create Project##CreateProjectModal"))
        {
            ImGui::Text("For now, you need to write the absolute location for the .atta file below");
            ImGui::Separator();

            static char buf[254] = "";
            ImGui::InputText("##createProjectAttaPath", buf, sizeof(buf));

            if(ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
                _showCreateProject = false;
                lastShow = false;
            }

            ImGui::SameLine();

            if(ImGui::Button("Create"))
            { 
                FileManager::createProject(fs::path(buf));
                ImGui::CloseCurrentPopup();
                _showCreateProject = false;
                lastShow = false;
            }
            ImGui::SetItemDefaultFocus();

            ImGui::EndPopup();
        }
    }

    void TopBar::viewportModals()
    {
        std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
        static std::vector<bool> newViewportModals;// If first time creating the modal
        _viewportModals.resize(viewports.size());

        // Check if first time creating viewport
        newViewportModals.resize(_viewportModals.size());
        for(unsigned i = 0; i < newViewportModals.size(); i++)
            newViewportModals[i] = !newViewportModals[i] && _viewportModals[i];

        for(uint32_t i = 0; i < _viewportModals.size(); i++)
        {
            char nameBuf[196];
            sprintf(nameBuf, "%s###ViewportProps%s", viewports[i]->getName().c_str(), viewports[i]->getSID().getString().c_str());

            bool open = _viewportModals[i];
            if(open)
            {
                if(newViewportModals[i])
                    ImGui::SetNextWindowSize(ImVec2(200.0f, 300.0f));
                if(ImGui::Begin(nameBuf, &open))
                {
                    viewports[i]->renderUI();

                    ImGui::Separator();
                    if(ImGui::Button("Delete Viewport"))
                    {
                        GraphicsManager::removeViewport(viewports[i]);
                        ImGui::End();
                        break;
                    }
                }
                ImGui::End();
                _viewportModals[i] = open;
            }
        }
        newViewportModals = _viewportModals;
    }
}
