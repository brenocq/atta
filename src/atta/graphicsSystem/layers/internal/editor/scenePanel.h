//--------------------------------------------------
// Atta Graphics System
// scenePanel.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_SCENE_PANEL_H
#define ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_SCENE_PANEL_H
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/entity.h>

namespace atta
{
	class ScenePanel
	{
	public:
		ScenePanel();

		void render();

		bool getSelected(EntityId& eid);
	private:
		void renderTree();
		void renderComponents();

		EntityId _selected;
		bool _someSelected;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_SCENE_PANEL_H

