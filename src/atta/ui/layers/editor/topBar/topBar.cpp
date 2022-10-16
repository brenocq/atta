//--------------------------------------------------
// Atta UI Module
// topBar.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/topBar/topBar.h>

#include <atta/ui/layers/editor/topBar/localWindows/versionWindow.h>

#include <atta/event/events/windowClose.h>
#include <atta/event/interface.h>
#include <atta/file/manager.h>
#include <atta/graphics/interface.h>
#include <imgui.h>

#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>

#include <atta/ui/layers/editor/moduleWindows/graphicsModuleWindow.h>
#include <atta/ui/layers/editor/moduleWindows/ioModuleWindow.h>
#include <atta/ui/layers/editor/moduleWindows/physicsModuleWindow.h>
#include <atta/ui/layers/editor/windows/utils/fileSelectionWindow.h>

namespace atta::ui {

TopBar::TopBar() : _showPreferences(false) {}

void TopBar::render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (file::isProjectOpen()) {
                ImGui::Text(file::getProject()->getName().c_str());
                ImGui::Separator();

#ifndef ATTA_STATIC_PROJECT
                if (ImGui::MenuItem("Close"))
                    _showSaveProject = true;
#endif
            }

#ifndef ATTA_STATIC_PROJECT
            if (ImGui::BeginMenu("Open")) {
                if (ImGui::MenuItem("From file")) {
                    FileSelectionWindow::setOpen(true);
                    _waitingChooseAttaFile = true;
                }
                if (ImGui::MenuItem("From published"))
                    _showOpenPublished = true;

                ImGui::EndMenu();
            }
#endif

            if (file::isProjectOpen())
                if (ImGui::MenuItem("Save"))
                    file::saveProject();

#ifndef ATTA_STATIC_PROJECT
            if (ImGui::MenuItem("Save as"))
                _showCreateProject = true;
#endif

            ImGui::Separator();

            if (ImGui::MenuItem("Quit")) {
                _showSaveProject = true;
                _quitAfterSaveModal = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Preferences"))
                _showPreferences = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Viewports")) {
            std::vector<std::shared_ptr<graphics::Viewport>> viewports = graphics::getViewports();
            _viewportModals.resize(viewports.size());
            int i = 0;
            for (auto viewport : viewports) {
                if (ImGui::MenuItem(viewport->getName().c_str()))
                    _viewportModals[i] = true;
                i++;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Create viewport")) {
                // Choose viewport name
                unsigned newViewportNumber = 0;
                bool found = false;
                while (!found) {
                    found = true;
                    for (auto viewport : viewports)
                        if (viewport->getSID() == StringId("Viewport " + std::to_string(newViewportNumber))) {
                            found = false;
                            break;
                        }
                    if (!found)
                        newViewportNumber++;
                }

                // Create viewport
                graphics::Viewport::CreateInfo viewportInfo;
                viewportInfo.renderer = std::make_shared<graphics::PhongRenderer>();
                viewportInfo.camera = std::static_pointer_cast<graphics::Camera>(
                    std::make_shared<graphics::PerspectiveCamera>(graphics::PerspectiveCamera::CreateInfo{}));
                viewportInfo.sid = StringId("Viewport " + std::to_string(newViewportNumber));
                std::shared_ptr<graphics::Viewport> viewport = std::make_shared<graphics::Viewport>(viewportInfo);
                graphics::addViewport(viewport);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Modules")) {
            if (ImGui::MenuItem("Graphics"))
                GraphicsModuleWindow::setOpen(true);
            if (ImGui::MenuItem("IO"))
                IOModuleWindow::setOpen(true);
            if (ImGui::MenuItem("Physics"))
                PhysicsModuleWindow::setOpen(true);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Version"))
                VersionWindow::setOpen(true);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
    openProjectModal();
    openPublishedWindow();
    createProjectModal();
    preferences();
    viewportModals();
    saveProjectModal();

    for (int i = (int)_repoWindows.size() - 1; i >= 0; i--) {
        RepoWindow& r = _repoWindows[i];
        r.render();
        if (r.getShouldClose())
            _repoWindows.erase(_repoWindows.begin() + i);
    }
}

void TopBar::preferences() {
    if (_showPreferences) {
        if (ImGui::Begin("Preferences", &_showPreferences)) {
            std::vector<std::string> stringIds = StringId::getStrings();
            for (const auto& str : stringIds)
                ImGui::Text(str.c_str());
        }
        ImGui::End();
    }
}

void TopBar::openProjectModal() {
    // Check if user chose some file
    if (_waitingChooseAttaFile && !FileSelectionWindow::getChosenFile().empty()) {
        _waitingChooseAttaFile = false;
        if (fs::exists(FileSelectionWindow::getChosenFile())) {
            file::saveProject();
            file::openProject(FileSelectionWindow::getChosenFile());
        }
    }
    // If was closed and no file was selected
    if (!FileSelectionWindow::getOpen())
        _waitingChooseAttaFile = false;
}

void TopBar::openPublishedWindow() {
    //#ifdef ATTA_CPPRESTSDK_SUPPORT
    //        static bool lastShow = false;
    //        std::string modalName = "Open Published##OpenPUblishedModal";
    //        static std::shared_ptr<http_client> client;
    //
    //        struct PublishedProject {
    //            // Get from json
    //            std::string repoUrl;
    //            std::string reponame;
    //            std::string username;
    //
    //            // Get from project readme
    //            std::string title;
    //            std::string description;
    //            std::string image;
    //            bool readmeInitialized;
    //            std::shared_ptr<http_client> readmeClient;
    //
    //            // Get image
    //            bool imageInitialized;
    //            std::shared_ptr<http_client> imageClient;
    //        };
    //        static std::vector<PublishedProject> publishedProjects;
    //
    //        if(_showOpenPublished && !lastShow)
    //        {
    //            // OBS: Doing this because can't open popup inside menuitem
    //            lastShow = _showOpenPublished;
    //
    //            // Open client and make request
    //            client = std::make_shared<http_client>("https://raw.githubusercontent.com/brenocq-atta/projects/main/projects.json");
    //            client->request(methods::GET)
    //                .then([](http_response response)
    //                {
    //                    if(response.status_code() == status_codes::OK)
    //                    {
    //                        response.headers().set_content_type(U("application/json"));
    //                        return response.extract_json();
    //                    }
    //                    return pplx::task<json::value>{};
    //                })
    //                .then([](const pplx::task<json::value>& task)
    //                {
    //                    try
    //                    {
    //                        json::value json = task.get();
    //
    //                        // Update published projects
    //                        publishedProjects.clear();
    //                        for(auto project : json.at("projects").as_array())
    //                        {
    //                            PublishedProject p;
    //                            p.repoUrl = project["repo"].as_string();
    //                            p.readmeInitialized = false;
    //                            p.imageInitialized = false;
    //
    //                            int start = 19;// https://github.com/
    //                            int userRepoDash = p.repoUrl.find('/', start);
    //                            p.username = p.repoUrl.substr(start, userRepoDash-start);
    //                            p.reponame = p.repoUrl.substr(userRepoDash+1, p.repoUrl.size()-userRepoDash+1);
    //                            std::string readmeUrl = "https://raw.githubusercontent.com/" + p.username + "/" + p.reponame + "/main/README.md";
    //
    //                            p.title = p.reponame;
    //                            p.description = "Project published by " + p.username;
    //
    //                            p.readmeClient = std::make_shared<http_client>(readmeUrl);
    //                            publishedProjects.push_back(p);
    //                        }
    //                    }
    //                    catch (const http_exception& e)
    //                    {
    //                        LOG_WARN("ui::TopBar", "Could not get published projects json: $0", e.what());
    //                    }
    //                    catch (const json::json_exception& e)
    //                    {
    //                        LOG_WARN("ui::TopBar", "Could not serialize published json: $0", e.what());
    //                    }
    //                });
    //        }
    //
    //        if(_showOpenPublished)
    //        {
    //            if(ImGui::Begin(modalName.c_str(), &_showOpenPublished))
    //            {
    //                // Check initialization
    //                for(PublishedProject& project : publishedProjects)
    //                {
    //                    // Get project information from readme metadata
    //                    if(!project.readmeInitialized)
    //                    {
    //                        project.readmeInitialized = true;
    //                        project.readmeClient->request(methods::GET)
    //                            .then([&](http_response response)
    //                            {
    //                                if(response.status_code() == status_codes::OK)
    //                                    return response.extract_string();
    //                            })
    //                            .then([&project](const pplx::task<string_t>& task)
    //                            {
    //                                std::string readmeStr = task.get();
    //                                // Check if readme contains metadata
    //                                if(readmeStr.substr(0,3) == "---")
    //                                {
    //                                    int start = 4;
    //                                    while(true)
    //                                    {
    //                                        size_t endMark = readmeStr.find(':', start);
    //                                        size_t nl = readmeStr.find('\n', start);
    //                                        size_t endMeta = readmeStr.find("---", start);
    //                                        if(endMark == std::string::npos ||
    //                                                nl == std::string::npos ||
    //                                                endMeta == std::string::npos)// File not well formated
    //                                            break;
    //                                        if(endMeta < nl)// End of metadata
    //                                            break;
    //
    //                                        std::string marker = readmeStr.substr(start, endMark-start);
    //                                        std::string value = readmeStr.substr(endMark+1, nl-endMark-1l);
    //                                        if(value[0] == ' ') value = value.substr(1, value.size()-1);
    //
    //                                        if(marker == "title")
    //                                            project.title = value;
    //                                        else if(marker == "description")
    //                                            project.description = value;
    //                                        else if(marker == "image")
    //                                            project.image = value;
    //
    //                                        start = nl+1;
    //                                    }
    //                                }
    //                            });
    //                    }
    //                }
    //
    //
    //                // Render projects
    //                ImGui::Text("Collection of published projects");
    //                ImGui::Separator();
    //
    //                if(ImGui::BeginTable("publishedProjectTable", 3, ImGuiTableFlags_Borders))
    //                {
    //                    ImGui::TableSetupColumn("desc", ImGuiTableColumnFlags_WidthStretch, 300.0f);
    //                    ImGui::TableSetupColumn("user", ImGuiTableColumnFlags_WidthFixed, 100.0f);
    //                    ImGui::TableSetupColumn("open", ImGuiTableColumnFlags_WidthFixed, 40.0f);
    //
    //                    for(PublishedProject& project : publishedProjects)
    //                    {
    //                        ImGui::TableNextColumn();
    //                        ImGui::Text(project.title.c_str());
    //                        ImGui::Spacing();
    //                        ImGui::Text(project.description.c_str());
    //
    //                        ImGui::TableNextColumn();
    //                        ImGui::Text(project.username.c_str());
    //
    //                        ImGui::TableNextColumn();
    //
    //                        fs::path pathToClone = file::getDefaultProjectFolder();
    //                        fs::path repoPath = pathToClone/project.reponame;
    //                        if(!fs::exists(repoPath))
    //                        {
    //                            // Clone repo if not cloned yet
    //                            if(ImGui::Button(("Clone###CloneButton"+project.repoUrl).c_str()))
    //                            {
    //                                fs::path prevPath = fs::current_path();
    //                                if(!fs::exists(pathToClone))
    //                                    fs::create_directories(pathToClone);
    //                                fs::current_path(pathToClone);
    //                                std::string command = "git clone git@github.com:" + project.username+"/"+project.reponame+".git";
    //                                LOG_VERBOSE("ui::TopBar", "Running command: $0", command);
    //                                std::system(command.c_str());
    //                                fs::current_path(prevPath);
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // Open repo project
    //                            if(ImGui::Button(("Open###OpenButton"+project.repoUrl).c_str()))
    //                            {
    //                                bool alreadyOpen = false;
    //                                for(auto& rw : _repoWindows)
    //                                    if(rw.getRepoPath() == repoPath)
    //                                    {
    //                                        alreadyOpen = true;
    //                                        break;
    //                                    }
    //
    //                                if(!alreadyOpen)
    //                                    _repoWindows.push_back(RepoWindow(repoPath));
    //                            }
    //                        }
    //
    //                        ImGui::TableNextRow();
    //                    }
    //
    //                    ImGui::EndTable();
    //                }
    //            }
    //            ImGui::End();
    //        }
    //#endif// ATTA_CPPRESTSDK_SUPPORT
}

void TopBar::createProjectModal() {
    static bool lastShow = false;
    if (_showCreateProject && !lastShow) {
        // OBS: Doing this because can't open popup inside menuitem
        ImGui::OpenPopup("Create Project##CreateProjectModal");
        lastShow = _showCreateProject;
    }

    if (ImGui::BeginPopupModal("Create Project##CreateProjectModal")) {
        ImGui::Text("For now, you need to write the absolute location for the .atta file below");
        ImGui::Separator();

        static char buf[254] = "";
        ImGui::InputText("##createProjectAttaPath", buf, sizeof(buf));

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            _showCreateProject = false;
            lastShow = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Create")) {
            file::createProject(fs::path(buf));
            ImGui::CloseCurrentPopup();
            _showCreateProject = false;
            lastShow = false;
        }
        ImGui::SetItemDefaultFocus();

        ImGui::EndPopup();
    }
}

void TopBar::saveProjectModal() {
    static bool lastShow = false;
    if (_showSaveProject && !lastShow) {
        // OBS: Doing this because can't open popup inside menuitem
        ImGui::OpenPopup("Save Project##SaveProjectModal");
        lastShow = _showSaveProject;
    }

    if (ImGui::BeginPopupModal("Save Project##SaveProjectModal")) {
        ImGui::Text("Do you want to save the project?");
        if (ImGui::Button("No")) {
            _showSaveProject = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Yes")) {
            _showSaveProject = false;
            file::saveProject();
        }
        ImGui::SetItemDefaultFocus();

        if (!_showSaveProject) {
            // Close project data
            file::closeProject();
            // Replace components with default
            component::clear();
            component::createDefault();
            // Replace viewports with default
            graphics::createDefaultViewports();

            if (_quitAfterSaveModal) {
                event::WindowClose e;
                event::publish(e);
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TopBar::viewportModals() {
    std::vector<std::shared_ptr<graphics::Viewport>> viewports = graphics::getViewports();
    static std::vector<bool> newViewportModals; // If first time creating the modal
    _viewportModals.resize(viewports.size());

    // Check if first time creating viewport
    newViewportModals.resize(_viewportModals.size());
    for (unsigned i = 0; i < newViewportModals.size(); i++)
        newViewportModals[i] = !newViewportModals[i] && _viewportModals[i];

    for (uint32_t i = 0; i < _viewportModals.size(); i++) {
        char nameBuf[196];
        sprintf(nameBuf, "%s###ViewportProps%s", viewports[i]->getName().c_str(), viewports[i]->getSID().getString().c_str());

        bool open = _viewportModals[i];
        if (open) {
            if (newViewportModals[i])
                ImGui::SetNextWindowSize(ImVec2(200.0f, 300.0f));
            if (ImGui::Begin(nameBuf, &open)) {
                viewports[i]->renderUI();

                ImGui::Separator();
                if (ImGui::Button("Delete Viewport")) {
                    graphics::removeViewport(viewports[i]);
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

} // namespace atta::ui
