//--------------------------------------------------
// Guib
// guiRender.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_RENDER_H
#define GUI_RENDER_H

#include <vector>
#include <string>
#include "guiPipelineLayout.h"
#include "widgets/widgets.h"

class GuiRender
{
	public:
		GuiRender(VkExtent2D _imageExtent, GuiPipelineLayout* _pipelineLayout);
		~GuiRender();

		void render(VkCommandBuffer commandBuffer, guib::Widget* root);
		void renderWidget(VkCommandBuffer commandBuffer, guib::Offset currOffset, guib::Size currSize, guib::Widget* widget);

		// Window callbacks
		void onKey(int key, int scancode, int action, int mods);
		void onCursorPosition(double xpos, double ypos);
		void onMouseButton(int button, int action, int mods);
		void onScroll(double xoffset, double yoffset);

	private:
		VkExtent2D _imageExtent;
		GuiPipelineLayout* _pipelineLayout;
		guib::Offset _cursorPos;

		//---------- GiuB widget handler ----------//
		std::vector<guib::ClickDetectorArea> _clickableAreas;
};

#endif// GUI_RENDER_H
