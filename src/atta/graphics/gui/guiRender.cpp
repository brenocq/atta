//--------------------------------------------------
// Guib
// guiRender.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/guiRender.h>
#include <atta/graphics/gui/guiStructs.h>
#include <atta/helpers/log.h>
#include <GLFW/glfw3.h>

namespace guib
{
	GuiRender::GuiRender(VkExtent2D imageExtent, std::shared_ptr<atta::GuiPipelineLayout> pipelineLayout, GLFWwindow* glfwWindow, std::shared_ptr<guib::FontLoader> fontLoader):
		_imageExtent(imageExtent), _pipelineLayout(pipelineLayout), _glfwWindow(glfwWindow),
		_root(nullptr)
	{
		state::guiRender = this;
		state::screenSize = {imageExtent.width, imageExtent.height};
		state::fontLoader = fontLoader.get();

		//---------- GLFW setup ----------//
		_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		glfwSetCursor(_glfwWindow, _cursor);
	}

	GuiRender::~GuiRender()
	{

	}

	void GuiRender::setRootWidget(guib::Widget* root)
	{
		_root = root;
		root->treePreProcess();
	}

	void GuiRender::setWindowWidgets(std::vector<guib::Window*> windows)
	{ 
		_windows = windows; 
		for(auto window : _windows)
			window->treePreProcess();
	}

	void GuiRender::render(VkCommandBuffer commandBuffer)//, guib::Widget* root, std::vector<guib::Window*> windows)
	{
		//guib::Offset currOffset = {0,0};
		//guib::Size currSize = {1,1};

		_commandBuffer = commandBuffer;// Widgets access this commandBuffer when necessary (box, text, ...)

		if(_root!=nullptr)
			_root->render();

		for(auto& window : _windows)
			window->render();
	}

	//----------------------------------------//
	//-------------- Callbacks ---------------//
	//----------------------------------------//
	void GuiRender::onKey(int key, int scancode, int action, int mods)
	{
	}

	void GuiRender::onCursorPosition(double xpos, double ypos)
	{
		state::cursorPos.x = xpos/_imageExtent.width;
		state::cursorPos.y = ypos/_imageExtent.height;
		////Log::debug("GuiRender", "cursorPos: "+_cursorPos.toString());
		//---------- Update dragging ----------//
		if(state::currDragging != nullptr)
		{
			guib::Offset diff = state::cursorPos-state::lastDraggingCursorPos;
			state::lastDraggingCursorPos = state::cursorPos;
			state::currDragging->getWidgetToDrag()->addOffsetTree(diff);
			//Log::debug("Widget", "New offset: [w]$0", state::currDragging->getWidgetToDrag()->getOffset().toString());
			updateCursor(CURSOR_TYPE_CROSSHAIR);
			return;
		}

		if(state::currClickArea.second)
		{
			//---------- Check current click detector ----------//
			// Check stopped hovering
			if(state::currClickArea.first.clickDetector->getHovering() && (
				state::cursorPos.x<state::currClickArea.first.offset.x ||
				state::cursorPos.y<state::currClickArea.first.offset.y ||
				state::cursorPos.x>state::currClickArea.first.offset.x+state::currClickArea.first.size.width ||
				state::cursorPos.y>state::currClickArea.first.offset.y+state::currClickArea.first.size.height))
			{
				state::currClickArea.first.clickDetector->setHovering(false);
				if(state::currClickArea.first.clickDetector->getOnStopHover())
					state::currClickArea.first.clickDetector->getOnStopHover()();
			}

			if(state::currClickArea.first.clickDetector->getHovering()==false && 
				state::cursorPos.x>=state::currClickArea.first.offset.x &&
				state::cursorPos.y>=state::currClickArea.first.offset.y &&
				state::cursorPos.x<=state::currClickArea.first.offset.x+state::currClickArea.first.size.width &&
				state::cursorPos.y<=state::currClickArea.first.offset.y+state::currClickArea.first.size.height)
			{
				state::currClickArea.first.clickDetector->setHovering(true);
				if(state::currClickArea.first.clickDetector->getOnHover())
					state::currClickArea.first.clickDetector->getOnHover()();
			}

			if(state::currClickArea.first.clickDetector->getClicking() == false &&
					state::currClickArea.first.clickDetector->getRightClicking() == false &&
					state::currClickArea.first.clickDetector->getHovering() == false)
				state::currClickArea.second = false;
		}
		else
		{
			//---------- Change cursor hover clickArea ----------//
			for(int i=(int)state::clickableAreas.size()-1; i>=0; i--)
			{
				guib::ClickDetectorArea &clickArea = state::clickableAreas[i];
				if(state::cursorPos.x>=clickArea.offset.x &&
					state::cursorPos.y>=clickArea.offset.y &&
					state::cursorPos.x<=clickArea.offset.x+clickArea.size.width &&
					state::cursorPos.y<=clickArea.offset.y+clickArea.size.height)
				{
					state::currClickArea.first = clickArea;
					state::currClickArea.second = true;
					if(!clickArea.clickDetector->getHovering());
					{
						clickArea.clickDetector->setHovering(true);
						if(clickArea.clickDetector->getOnHover())
							clickArea.clickDetector->getOnHover()();
					}
					break;
				}
			}
		}

		//---------- Check hovering click area ----------//
		if(state::currClickArea.second && state::currClickArea.first.clickDetector->getHovering())
		{
			updateCursor(CURSOR_TYPE_HAND);
			return;
		}

		//---------- Check hovering draggable area ----------//
		{
			// Check draggable
			guib::Draggable* draggable = nullptr;
			for(int i=(int)state::draggables.size()-1; i>=0; i--)
			{
				guib::Offset dragOffset = state::draggables[i]->getWidgetToHover()->getOffset();
				guib::Size dragSize = state::draggables[i]->getWidgetToHover()->getSize();
				if(state::cursorPos.x>=dragOffset.x &&
					state::cursorPos.y>=dragOffset.y &&
					state::cursorPos.x<=dragOffset.x+dragSize.width &&
					state::cursorPos.y<=dragOffset.y+dragSize.height)
				{
					draggable = state::draggables[i];
					break;
				}
			}

			if(draggable != nullptr && draggable->getActive())
			{
				updateCursor(CURSOR_TYPE_CROSSHAIR);
			}
			else
				updateCursor(CURSOR_TYPE_ARROW);
		}

	}

	void GuiRender::onMouseButton(int button, int action, int mods)
	{
		if(state::currClickArea.second)
		{
			//---------- Update current click detector ----------//
			// Update current click detector if exists (mouse pressing, hovering, release...)
			if(action == GLFW_RELEASE)
			{
				// Run stop clicking function
				if(button == GLFW_MOUSE_BUTTON_LEFT && state::currClickArea.first.clickDetector->getClicking())
				{
					state::currClickArea.first.clickDetector->setClicking(false);
					if(state::currClickArea.first.clickDetector->getOnStopClick())
						state::currClickArea.first.clickDetector->getOnStopClick()();
				}
				else if(button == GLFW_MOUSE_BUTTON_RIGHT && state::currClickArea.first.clickDetector->getRightClicking())
				{
					state::currClickArea.first.clickDetector->setRightClicking(false);
					if(state::currClickArea.first.clickDetector->getOnStopRightClick())
						state::currClickArea.first.clickDetector->getOnStopRightClick()();
				}
			}
			if(action == GLFW_PRESS)
			{
				// Run start clicking function
				if(button == GLFW_MOUSE_BUTTON_LEFT)
				{
					state::currClickArea.first.clickDetector->setClicking(true);
					if(state::currClickArea.first.clickDetector->getOnClick())
						state::currClickArea.first.clickDetector->getOnClick()();
				}
				else if(button == GLFW_MOUSE_BUTTON_RIGHT)
				{
					state::currClickArea.first.clickDetector->setRightClicking(true);
					if(state::currClickArea.first.clickDetector->getOnRightClick())
						state::currClickArea.first.clickDetector->getOnRightClick()();
				}
			}

			// If stopped clicking/hovering, no current ClickDetect
			if(state::currClickArea.first.clickDetector->getClicking() == false &&
					state::currClickArea.first.clickDetector->getRightClicking() == false &&
					state::currClickArea.first.clickDetector->getHovering() == false)
				state::currClickArea.second = false;
		}
		else
		{
			//---------- Search for click in click detector ----------//
			for(int i=(int)state::clickableAreas.size()-1; i>=0; i--)
			{
				guib::ClickDetectorArea clickArea = state::clickableAreas[i];
				if(state::cursorPos.x>=clickArea.offset.x &&
					state::cursorPos.y>=clickArea.offset.y &&
					state::cursorPos.x<=clickArea.offset.x+clickArea.size.width &&
					state::cursorPos.y<=clickArea.offset.y+clickArea.size.height)
				{
					// Found click in click detector
					state::currClickArea.first = clickArea;
					state::currClickArea.second = true;

					// First click
					if(action == GLFW_PRESS)
					{
						if(button == GLFW_MOUSE_BUTTON_LEFT)
						{
							state::currClickArea.first.clickDetector->setClicking(true);
							if(state::currClickArea.first.clickDetector->getOnClick())
								state::currClickArea.first.clickDetector->getOnClick()();
						}
						else if(button == GLFW_MOUSE_BUTTON_RIGHT)
						{
							state::currClickArea.first.clickDetector->setRightClicking(true);
							if(state::currClickArea.first.clickDetector->getOnRightClick())
								state::currClickArea.first.clickDetector->getOnRightClick()();
						}
					}

					break;
				}
			}
		}

		//---------- Check draggable ----------//
		if(state::currDragging != nullptr)
		{
			// Check stop dragging
			if(action == GLFW_RELEASE)
			{
				//updateCursor(CURSOR_TYPE_ARROW);
				state::currDragging = nullptr;
				state::lastDraggingCursorPos = {0,0};
			}
		}
		else
		{
			// Check start dragging
			if(action == GLFW_PRESS)
			{
				// Check if some draggable was pressed
				guib::Draggable* draggable = nullptr;
				for(int i=(int)state::draggables.size()-1; i>=0; i--)
				{
					guib::Offset dragOffset = state::draggables[i]->getWidgetToHover()->getOffset();
					guib::Size dragSize = state::draggables[i]->getWidgetToHover()->getSize();
					if(state::cursorPos.x>=dragOffset.x &&
						state::cursorPos.y>=dragOffset.y &&
						state::cursorPos.x<=dragOffset.x+dragSize.width &&
						state::cursorPos.y<=dragOffset.y+dragSize.height)
					{
						draggable = state::draggables[i];
						break;
					}
				}

				if(draggable != nullptr && draggable->getActive())
				{
					if(action == GLFW_PRESS)
					{
						state::currDragging = draggable;
						state::lastDraggingCursorPos = state::cursorPos;
					}
				}
			}
		}

	}

	void GuiRender::onScroll(double xoffset, double yoffset)
	{

	}

	void GuiRender::updateCursor(CursorType cursorType)
	{
		if(cursorType != state::cursorType)
		{
			state::cursorType = cursorType;
			glfwDestroyCursor(_cursor);
			switch(state::cursorType)
			{
				case CURSOR_TYPE_ARROW:
					_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
					break;
				case CURSOR_TYPE_TEXT:
					_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
					break;
				case CURSOR_TYPE_HAND:
					_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
					break;
				case CURSOR_TYPE_CROSSHAIR:
					_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
					break;
				case CURSOR_TYPE_HRESIZE:
					_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
					break;
				case CURSOR_TYPE_VRESIZE:
					_cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
					break;
				default:
					_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
					break;
			}
			glfwSetCursor(_glfwWindow, _cursor);
		}
	}
}
