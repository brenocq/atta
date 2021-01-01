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
#include "guiState.h"

namespace guib
{
	class GuiRender
	{
		public:
			GuiRender(VkExtent2D _imageExtent, GuiPipelineLayout* _pipelineLayout, GLFWwindow* glfwWindow, guib::FontLoader* fontLoader);
			~GuiRender();

			void render(VkCommandBuffer commandBuffer);
			//void renderWidget(VkCommandBuffer commandBuffer, guib::Offset currOffset, guib::Size currSize, guib::Widget* widget);

			// Window callbacks
			void onKey(int key, int scancode, int action, int mods);
			void onCursorPosition(double xpos, double ypos);
			void onMouseButton(int button, int action, int mods);
			void onScroll(double xoffset, double yoffset);

			//---------- Getters and Setters ----------//
			GuiPipelineLayout* getPipelineLayout() const { return _pipelineLayout; }
			VkCommandBuffer getCommandBuffer() const { return _commandBuffer; }

			void setRootWidget(guib::Widget* root);
			void setWindowWidgets(std::vector<guib::Window*> windows);

		private:
			//void preProcessWidgetTree(guib::Widget* root);
			void updateCursor(CursorType cursorType);

			GuiPipelineLayout* _pipelineLayout;
			VkCommandBuffer _commandBuffer;

			//---------- GuiB window/viewport handling ----------//
			VkExtent2D _imageExtent;
			GLFWwindow* _glfwWindow;
			GLFWcursor* _cursor;
			//guib::Offset _cursorPos;
			//CursorType _cursorType;

			//---------- GuiB special widget handler ----------//
			// Handling ClickDetector
			//std::vector<guib::ClickDetectorArea> _clickableAreas;
			//std::pair<guib::ClickDetectorArea,bool> _currClickArea;// Current hovering/clicking
			//// Handling Draggable
			//std::vector<guib::DragDetectorArea> _draggableAreas;
			//guib::Draggable* _currDragging;
			//guib::Offset _startDraggingOffset;
			//guib::Offset _startDraggingCursorPos;

			//---------- GuiB  ----------//
			//guib::FontLoader* _fontLoader;
			guib::Widget* _root;
			std::vector<guib::Window*> _windows;
	};
}

#endif// GUI_RENDER_H
