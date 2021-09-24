//--------------------------------------------------
// Atta Graphics System
// menuBar.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/menuBar.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/fileSystem/fileManager.h>
#include <imgui_internal.h>

#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/renderers/phongRenderer.h>
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>

namespace atta
{
	MenuBar::MenuBar():
		_showPreferences(false)
	{

	}

	void MenuBar::render()
	{
		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{

				if(FileManager::isProjectOpen())
				{
					ImGui::Text(FileManager::getProject()->getName().c_str());
					if(ImGui::MenuItem("Close"))
						FileManager::closeProject();
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
				for(auto& viewport : viewports)
				{
					if(ImGui::MenuItem(viewport->getSID().getString().c_str()))
						_viewportModals[i] = true;
					i++;
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Create new"))
				{
					Viewport::CreateInfo viewportInfo;
					viewportInfo.renderer = std::make_shared<PhongRenderer>();
					viewportInfo.camera = std::static_pointer_cast<Camera>(std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{}));
					viewportInfo.sid = StringId("New Viewport");
					std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(viewportInfo);
					GraphicsManager::addViewport(viewport);
				}


				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		openProjectModal();
		createProjectModal();
		preferences();
		viewportModals();
	}

	void MenuBar::preferences()
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

	void MenuBar::openProjectModal()
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
				FileManager::openProject(fs::path(buf));
				ImGui::CloseCurrentPopup();
				_showOpenProject = false;
				lastShow = false;
			}
            ImGui::SetItemDefaultFocus();

            ImGui::EndPopup();
        }
	}

	void MenuBar::createProjectModal()
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
				FileManager::saveNewProject(fs::path(buf));
				ImGui::CloseCurrentPopup();
				_showCreateProject = false;
				lastShow = false;
			}
            ImGui::SetItemDefaultFocus();

            ImGui::EndPopup();
        }
	}

	void MenuBar::viewportModals()
	{
		std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();

		for(uint32_t i = 0; i < _viewportModals.size(); i++)
		{
			if(_viewportModals[i])
			{
				std::string viewportName = viewports[i]->getSID().getString();
				if(ImGui::Begin((viewportName+" Properties").c_str()))
				{
					ImGui::Text("Name");
					ImGui::Text(viewportName.c_str());
					ImGui::Separator();

					ImGui::Text("Renderer");
					std::vector<const char*> renderers = { "FastRenderer", "PhongRenderer", "PbrRenderer" };

					int comboValue = 0;
					for(size_t j = 0; j < renderers.size(); j++)
					{
						if(viewports[i]->getRenderer()->getName() == renderers[j])
						{
							comboValue = j;
							break;
						}
					}

					const char* comboPreviewValue = renderers[comboValue];
					if(ImGui::BeginCombo("Renderer", comboPreviewValue))
					{
						for(size_t j = 0; j < renderers.size(); j++)
						{
							if(ImGui::Selectable(renderers[j], comboValue == j))
							{
								switch(j)
								{
									case 0:
										viewports[i]->setRenderer(std::make_shared<FastRenderer>());
										break;
									case 1:
										viewports[i]->setRenderer(std::make_shared<PhongRenderer>());
										break;
									case 2:
										viewports[i]->setRenderer(std::make_shared<PbrRenderer>());
										break;
								}
							}
							if(comboValue == j)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					ImGui::Separator();

					ImGui::Text("Camera");

					ImGui::End();
				}
			}
		}
	}
}
