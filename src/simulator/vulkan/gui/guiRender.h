//--------------------------------------------------
// Guib
// guiRender.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_RENDER_H
#define GUI_RENDER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "guiPipelineLayout.h"
#include "widgets/widgets.h"
#include "font/fontLoader.h"

class GuiRender
{
	enum CursorType
	{
		CURSOR_TYPE_ARROW,
		CURSOR_TYPE_TEXT,
		CURSOR_TYPE_HAND,
		CURSOR_TYPE_CROSSHAIR,
		CURSOR_TYPE_HRESIZE,
		CURSOR_TYPE_VRESIZE
	};

	public:
		GuiRender(VkExtent2D _imageExtent, GuiPipelineLayout* _pipelineLayout, GLFWwindow* glfwWindow, guib::FontLoader* fontLoader);
		~GuiRender();

		void render(VkCommandBuffer commandBuffer, guib::Widget* root = nullptr, std::vector<guib::Window*> windows = std::vector<guib::Window*>());
		void renderWidget(VkCommandBuffer commandBuffer, guib::Offset currOffset, guib::Size currSize, guib::Widget* widget);

		// Window callbacks
		void onKey(int key, int scancode, int action, int mods);
		void onCursorPosition(double xpos, double ypos);
		void onMouseButton(int button, int action, int mods);
		void onScroll(double xoffset, double yoffset);

	private:
		void updateCursor(CursorType cursorType);
		GuiPipelineLayout* _pipelineLayout;

		//---------- GuiB window/viewport handling ----------//
		VkExtent2D _imageExtent;
		GLFWwindow* _glfwWindow;
		GLFWcursor* _cursor;
		guib::Offset _cursorPos;
		CursorType _cursorType;

		//---------- GuiB widget handler ----------//
		std::vector<guib::ClickDetectorArea> _clickableAreas;
		std::vector<guib::DragDetectorArea> _draggableAreas;
		// Handling ClickDetector
		std::pair<guib::ClickDetectorArea,bool> _currClickArea;// Current hovering/clicking
		// Handling Draggable
		guib::Draggable* _currDragging;
		guib::Offset _startDraggingOffset;
		guib::Offset _startDraggingCursorPos;

		//---------- GuiB font render ----------//
		guib::FontLoader* _fontLoader;
};

#endif// GUI_RENDER_H
