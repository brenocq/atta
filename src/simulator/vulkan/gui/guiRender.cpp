//--------------------------------------------------
// Guib
// guiRender.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "guiRender.h"
#include "guiStructs.h"
#include "simulator/helpers/log.h"
#include <GLFW/glfw3.h>

GuiRender::GuiRender(VkExtent2D imageExtent, GuiPipelineLayout* pipelineLayout, GLFWwindow* glfwWindow, guib::FontLoader* fontLoader):
	_imageExtent(imageExtent), _pipelineLayout(pipelineLayout), _glfwWindow(glfwWindow), 
	_cursorPos({0,0}), _cursorType(CURSOR_TYPE_ARROW), _currDragging(nullptr),
	_fontLoader(fontLoader)
{
	guib::Widget::screenSize = {imageExtent.width, imageExtent.height};

	//---------- GLFW setup ----------//
	_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	glfwSetCursor(_glfwWindow, _cursor);
}

GuiRender::~GuiRender()
{

}

void GuiRender::render(VkCommandBuffer commandBuffer, guib::Widget* root, std::vector<guib::Window*> windows)
{
	guib::Offset currOffset = {0,0};
	guib::Size currSize = {1,1};
	_clickableAreas.clear();
	_draggableAreas.clear();

	if(root!=nullptr)
		renderWidget(commandBuffer, currOffset, currSize, root);

	for(auto& window : windows)
		renderWidget(commandBuffer, currOffset, currSize, window);
}

void GuiRender::renderWidget(VkCommandBuffer commandBuffer, guib::Offset currOffset, guib::Size currSize, guib::Widget* widget)
{
	if(widget == nullptr)
		return;

	// Update size and offset 
	{
		guib::Size size = widget->getSize();
		if(size.unitW == guib::UNIT_PIXEL)
		{
			size.width /= _imageExtent.width*currSize.width;
			size.unitW = guib::UNIT_PERCENT;
		}
		if(size.unitH == guib::UNIT_PIXEL)
		{
			size.height /= _imageExtent.height*currSize.height;
			size.unitH = guib::UNIT_PERCENT;
		}

		guib::Offset offset = widget->getOffset();
		if(offset.unitX == guib::UNIT_PIXEL)
		{
			offset.x /= _imageExtent.width*currSize.width;
			offset.unitX = guib::UNIT_PERCENT;
		}
		if(offset.unitY == guib::UNIT_PIXEL)
		{
			offset.y /= _imageExtent.height*currSize.height;
			offset.unitY = guib::UNIT_PERCENT;
		}
		currSize*=size;
		currOffset+=offset;
	}

	std::string type = widget->getType();
	if(type=="Box")
	{
		guib::Box* box = (guib::Box*)widget;
		guib::Color color = box->getColor();

		GuiObjectInfo objectInfo;
		objectInfo.position = glm::vec2(currOffset.x, currOffset.y);
		objectInfo.size = glm::vec2(currSize.width, currSize.height);
		objectInfo.color = glm::vec4(color.r, color.g, color.b, color.a);
		objectInfo.isLetter = 0;

		// Calculate radius
		float minSize = std::min(currSize.height, currSize.width);
		objectInfo.radius = box->getRadius().topLeft*minSize*2;

		vkCmdPushConstants(
				commandBuffer,
				_pipelineLayout->handle(),
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(GuiObjectInfo),
				&objectInfo);

		vkCmdDraw(commandBuffer, 6, 1, 0, 0);

		renderWidget(commandBuffer, currOffset, currSize, widget->getChild());
	}
	else if(type=="Text")
	{
		guib::Text* text = (guib::Text*)widget;
		guib::Color color = text->getColor();

		float currX = currOffset.x;
		//std::cout << "Render text: " << text->getText() << std::endl;
		//std::cout << "Size: " << currSize.toString() << std::endl;
		//std::cout << "Offset: " << currOffset.toString() << std::endl;
		for(auto letter : text->getText())
		{
			
			guib::GlyphInfo gInfo = _fontLoader->getFontTexture().glyphsInfo[letter];
			float tw = gInfo.width;
			float th = gInfo.height;
			float tx = gInfo.x;
			float ty = gInfo.y;
			float heightPerc = gInfo.height*_fontLoader->getFontTexture().atlas.height/100.0f;
			float offsetLeft = gInfo.left/gInfo.width;
			float offsetTop = (gInfo.top/gInfo.height);
			float advance = gInfo.advance/gInfo.width;

			//std::cout << "offset " << offsetTop << std::endl;

			glm::vec2 textOffset = {currX, currOffset.y};
			textOffset.y += currSize.height*(1-heightPerc);

			GuiObjectInfo objectInfo;
			objectInfo.position = textOffset;
			objectInfo.size = glm::vec2(currSize.height*(tw/th)*heightPerc, currSize.height*heightPerc);
			objectInfo.color = glm::vec4(color.r, color.g, color.b, color.a);
			objectInfo.isLetter = 1;
			objectInfo.offsetLetter = glm::vec2(tx, ty);
			objectInfo.sizeLetter = glm::vec2(tw, th);

			vkCmdPushConstants(
					commandBuffer,
					_pipelineLayout->handle(),
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
					0,
					sizeof(GuiObjectInfo),
					&objectInfo);

			vkCmdDraw(commandBuffer, 6, 1, 0, 0);

			currX += currSize.height*(tw/th)*heightPerc;
		}
	}
	else if(type=="Column")
	{
		guib::Column* column = (guib::Column*)widget;
		std::vector<guib::Widget*> children = column->getChildren();
		guib::Size totalSize = column->getChildrenTotalSize();

		switch(column->getVAlignment())
		{
			case guib::ALIGN_START:
				break;
			case guib::ALIGN_CENTER:
				{
					float offset = currSize.height/2-totalSize.height/2;
					currOffset.y += offset;
					currSize.height -= offset;
				}
				break;
			case guib::ALIGN_END:
				{
					float offset = currSize.height-totalSize.height;
					currOffset.y += offset;
					currSize.height -= offset;
				}
				break;
		}


		for(auto& child : children)
		{
			guib::Size childSize = child->getSize();
			guib::Offset childOffset = currOffset;
			if(childSize.unitW == guib::UNIT_PIXEL)
			{
				childSize.width /= _imageExtent.width*currSize.width;
				childSize.unitW = guib::UNIT_PERCENT;
			}
			if(childSize.unitH == guib::UNIT_PIXEL)
			{
				childSize.height /= _imageExtent.height*currSize.height;
				childSize.unitH = guib::UNIT_PERCENT;
			}

			if(column->getHAlignment()==guib::ALIGN_CENTER)
				childOffset.x+=currSize.width/2-childSize.width*currSize.width/2;
			else if(column->getHAlignment()==guib::ALIGN_END)
				childOffset.x+=currSize.width-childSize.width*currSize.width;

			renderWidget(commandBuffer, childOffset, currSize, child);

			float offset = currSize.height*childSize.height;
			currOffset.y+=offset;
			currSize.height-=offset;
		}
	}
	else if(type=="Row")
	{
		guib::Row* row = (guib::Row*)widget;
		std::vector<guib::Widget*> children = row->getChildren();
		guib::Size totalSize = row->getChildrenTotalSize();

		switch(row->getHAlignment())
		{
			case guib::ALIGN_START:
				break;
			case guib::ALIGN_CENTER:
				{
					float offset = currSize.width/2-totalSize.width/2;
					currOffset.x += offset;
					currSize.width -= offset;
				}
				break;
			case guib::ALIGN_END:
				{
					float offset = currSize.width-totalSize.width;
					currOffset.x += offset;
					currSize.width -= offset;
				}
				break;
		}

		for(auto& child : children)
		{
			guib::Size childSize = child->getSize();
			guib::Offset childOffset = currOffset;
			if(childSize.unitW == guib::UNIT_PIXEL)
			{
				childSize.width /= _imageExtent.width*currSize.width;
				childSize.unitW = guib::UNIT_PERCENT;
			}
			if(childSize.unitH == guib::UNIT_PIXEL)
			{
				childSize.height /= _imageExtent.height*currSize.height;
				childSize.unitH = guib::UNIT_PERCENT;
			}

			if(row->getVAlignment()==guib::ALIGN_CENTER)
				childOffset.y+=currSize.height/2-childSize.height*currSize.height/2;
			else if(row->getVAlignment()==guib::ALIGN_END)
				childOffset.y+=currSize.height-childSize.height*currSize.height;

			renderWidget(commandBuffer, childOffset, currSize, child);

			float offset = currSize.width*childSize.width;
			currOffset.x+=offset;
			currSize.width-=offset;
		}
	}
	else if(type=="Padding")
	{
		guib::Padding* padding = (guib::Padding*)widget;
		guib::PaddingValues pad = padding->getPadding();
		guib::Widget* child = padding->getChild();
		if(child!=nullptr)
		{
			currOffset.x+=currSize.width*pad.left;
			currOffset.y+=currSize.height*pad.top;

			currSize.width-=(currSize.width*pad.left+currSize.width*pad.right);
			currSize.height-=(currSize.height*pad.top+currSize.height*pad.bottom);
			renderWidget(commandBuffer, currOffset, currSize, child);
		}
	}
	else if(type=="ClickDetector")
	{
		guib::ClickDetector* clickDetector = (guib::ClickDetector*)widget;
		guib::Widget* child = clickDetector->getChild();

		_clickableAreas.push_back({
					currOffset,
					currSize,
					clickDetector
				});

		renderWidget(commandBuffer, currOffset, currSize, child);
	}
	else if(type=="Draggable")
	{
		guib::Draggable* draggable = (guib::Draggable*)widget;
		guib::Widget* child = draggable->getChild();

		if(draggable->getActive())
		{
			guib::Offset dragOffset = draggable->getDragAreaOffset();
			if(dragOffset.unitX == guib::UNIT_PIXEL)
			{
				dragOffset.x /= _imageExtent.width*currSize.width;
				dragOffset.unitX = guib::UNIT_PERCENT;
			}
			else
				dragOffset.x *= currSize.width;
			if(dragOffset.unitY == guib::UNIT_PIXEL)
			{
				dragOffset.y /= _imageExtent.height*currSize.height;
				dragOffset.unitY = guib::UNIT_PERCENT;
			}
			else
				dragOffset.y *= currSize.height;

			guib::Size dragSize = draggable->getDragAreaSize();
			if(dragSize.unitW == guib::UNIT_PIXEL)
			{
				dragSize.width /= _imageExtent.width;
				dragSize.unitW = guib::UNIT_PERCENT;
			}
			else
				dragSize.width *= currSize.width;

			if(dragSize.unitH == guib::UNIT_PIXEL)
			{
				dragSize.height /= _imageExtent.height;
				dragSize.unitH = guib::UNIT_PERCENT;
			}
			else
				dragSize.height *= currSize.height;

			if(dragSize.width+dragOffset.x>currSize.width)
				dragSize.width = std::max(0.0f, currSize.width-dragOffset.x);
			if(dragSize.height+dragOffset.y>currSize.height)
				dragSize.height = std::max(0.0f, currSize.height-dragOffset.y);

			_draggableAreas.push_back({
						currOffset+dragOffset,
						dragSize,
						draggable
					});
		}

		renderWidget(commandBuffer, currOffset, currSize, child);
	}
	else if(type=="Window")
	{
		renderWidget(commandBuffer, currOffset, currSize, widget->getChild());
	}
	else if(type=="Visibility")
	{
		guib::Visibility* visibility = (guib::Visibility*)widget;
		guib::Widget* child = visibility->getChild();

		if(visibility->getVisible())
		{
			renderWidget(commandBuffer, currOffset, currSize, child);
		}
	}
	else if(type=="Protect")
	{
		guib::Protect* protect = (guib::Protect*)widget;
		guib::Widget* child = protect->getProtectedChild();

		renderWidget(commandBuffer, currOffset, currSize, child);
	}
	else if(type=="Button")
	{
		renderWidget(commandBuffer, currOffset, currSize, widget->getChild());
	}
	else if(type=="Align")
	{
		guib::Align* align = (guib::Align*)widget;
		guib::Widget* child = align->getChild();
		guib::Size childSize = child->getSize();
		if(childSize.unitW == guib::UNIT_PIXEL)
		{
			childSize.width /= _imageExtent.width;
			childSize.unitW = guib::UNIT_PERCENT;
		}
		else
			childSize.width *= currSize.width;

		if(childSize.unitH == guib::UNIT_PIXEL)
		{
			childSize.height /= _imageExtent.height;
			childSize.unitH = guib::UNIT_PERCENT;
		}
		else
			childSize.height *= currSize.height;

		switch(align->getHAlignment())
		{
			case guib::ALIGN_START:
				break;
			case guib::ALIGN_CENTER:
				{
					float offset = currSize.width/2-childSize.width/2;
					currOffset.x += offset;
					currSize.width -= offset;
				}
				break;
			case guib::ALIGN_END:
				{
					float offset = currSize.width-childSize.width;
					currOffset.x += offset;
					currSize.width -= offset;
				}
				break;
		}

		switch(align->getVAlignment())
		{
			case guib::ALIGN_START:
				break;
			case guib::ALIGN_CENTER:
				{
					float offset = currSize.height/2-childSize.height/2;
					currOffset.y += offset;
					currSize.height -= offset;
				}
				break;
			case guib::ALIGN_END:
				{
					float offset = currSize.height-childSize.height;
					currOffset.y += offset;
					currSize.height -= offset;
				}
				break;
		}

		renderWidget(commandBuffer, currOffset, currSize, widget->getChild());
	}
}

//----------------------------------------//
//-------------- Callbacks ---------------//
//----------------------------------------//
void GuiRender::onKey(int key, int scancode, int action, int mods)
{
}

void GuiRender::onCursorPosition(double xpos, double ypos)
{
	_cursorPos.x = xpos/_imageExtent.width;
	_cursorPos.y = ypos/_imageExtent.height;
	//Log::debug("GuiRender", "cursorPos: "+_cursorPos.toString());
	//---------- Update dragging ----------//
	if(_currDragging != nullptr)
	{
		guib::Offset diff = _cursorPos-_startDraggingCursorPos;
		_currDragging->getWidgetToDrag()->setOffset(_startDraggingOffset+diff);
		updateCursor(CURSOR_TYPE_CROSSHAIR);
		return;
	}

	//---------- Check current click detector ----------//
	if(_currClickArea.second)
	{
		// Check stopped hovering
		if(_cursorPos.x<_currClickArea.first.offset.x ||
			_cursorPos.y<_currClickArea.first.offset.y ||
			_cursorPos.x>_currClickArea.first.offset.x+_currClickArea.first.size.width ||
			_cursorPos.y>_currClickArea.first.offset.y+_currClickArea.first.size.height)
		{
			_currClickArea.first.clickDetector->setHovering(false);
			if(_currClickArea.first.clickDetector->getOnStopHover())
				_currClickArea.first.clickDetector->getOnStopHover()();
		}

		if(_currClickArea.first.clickDetector->getClicking() == false &&
				_currClickArea.first.clickDetector->getRightClicking() == false &&
				_currClickArea.first.clickDetector->getHovering() == false)
			_currClickArea.second = false;
	}

	//---------- Change cursor ----------//
	if(_currClickArea.second == false)
	{
		// Check click detector
		for(int i=(int)_clickableAreas.size()-1; i>=0; i--)
		{
			guib::ClickDetectorArea clickArea = _clickableAreas[i];
			if(_cursorPos.x>=clickArea.offset.x &&
				_cursorPos.y>=clickArea.offset.y &&
				_cursorPos.x<=clickArea.offset.x+clickArea.size.width &&
				_cursorPos.y<=clickArea.offset.y+clickArea.size.height)
			{
				_currClickArea.first = clickArea;
				_currClickArea.second = true;
				_currClickArea.first.clickDetector->setHovering(true);
				if(_currClickArea.first.clickDetector->getOnHover())
					_currClickArea.first.clickDetector->getOnHover()();
				break;
			}
		}
	}

	//---------- Check hovering click area ----------//
	if(_currClickArea.second && _currClickArea.first.clickDetector->getHovering())
	{
		updateCursor(CURSOR_TYPE_HAND);
	}
	//---------- Check hovering draggable area ----------//
	else
	{
		// Check draggable
		guib::Draggable* draggable = nullptr;
		for(int i=(int)_draggableAreas.size()-1; i>=0; i--)
		{
			guib::DragDetectorArea dragArea = _draggableAreas[i];
			if(_cursorPos.x>=dragArea.offset.x &&
				_cursorPos.y>=dragArea.offset.y &&
				_cursorPos.x<=dragArea.offset.x+dragArea.size.width &&
				_cursorPos.y<=dragArea.offset.y+dragArea.size.height)
			{
				draggable = dragArea.draggable;
				break;
			}
		}

		if(draggable != nullptr)
		{
			updateCursor(CURSOR_TYPE_CROSSHAIR);
		}
		else
			updateCursor(CURSOR_TYPE_ARROW);
	}

}

void GuiRender::onMouseButton(int button, int action, int mods)
{
	//---------- Check current click detector ----------//
	// Update current click detector if exists (mouse pressing, hovering, release...)
	if(_currClickArea.second)
	{
		if(action == GLFW_RELEASE)
		{
			if(button == GLFW_MOUSE_BUTTON_LEFT && _currClickArea.first.clickDetector->getClicking())
			{
				_currClickArea.first.clickDetector->setClicking(false);
				if(_currClickArea.first.clickDetector->getOnStopClick())
					_currClickArea.first.clickDetector->getOnStopClick()();
			}
			else if(button == GLFW_MOUSE_BUTTON_RIGHT && _currClickArea.first.clickDetector->getRightClicking())
			{
				_currClickArea.first.clickDetector->setRightClicking(false);
				if(_currClickArea.first.clickDetector->getOnStopRightClick())
					_currClickArea.first.clickDetector->getOnStopRightClick()();
			}
		}
		if(_currClickArea.first.clickDetector->getClicking() == false &&
				_currClickArea.first.clickDetector->getRightClicking() == false &&
				_currClickArea.first.clickDetector->getHovering() == false)
			_currClickArea.second = false;
	}

	//---------- Check click detector ----------//
	for(int i=(int)_clickableAreas.size()-1; i>=0; i--)
	{
		guib::ClickDetectorArea clickArea = _clickableAreas[i];
		if(_cursorPos.x>=clickArea.offset.x &&
			_cursorPos.y>=clickArea.offset.y &&
			_cursorPos.x<=clickArea.offset.x+clickArea.size.width &&
			_cursorPos.y<=clickArea.offset.y+clickArea.size.height)
		{
			_currClickArea.first = clickArea;
			_currClickArea.second = true;
			break;
		}
	}
	if(_currClickArea.second)
	{
		if(action == GLFW_PRESS)
		{
			if(button == GLFW_MOUSE_BUTTON_LEFT)
			{
				_currClickArea.first.clickDetector->setClicking(true);
				if(_currClickArea.first.clickDetector->getOnClick())
					_currClickArea.first.clickDetector->getOnClick()();
			}
			else if(button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				_currClickArea.first.clickDetector->setRightClicking(true);
				if(_currClickArea.first.clickDetector->getOnRightClick())
					_currClickArea.first.clickDetector->getOnRightClick()();
			}
		}
	}

	//---------- Check draggable ----------//
	guib::Draggable* draggable = nullptr;
	for(int i=(int)_draggableAreas.size()-1; i>=0; i--)
	{
		guib::DragDetectorArea dragArea = _draggableAreas[i];
		if(_cursorPos.x>=dragArea.offset.x &&
			_cursorPos.y>=dragArea.offset.y &&
			_cursorPos.x<=dragArea.offset.x+dragArea.size.width &&
			_cursorPos.y<=dragArea.offset.y+dragArea.size.height)
		{
			draggable = dragArea.draggable;
			break;
		}
	}

	if(action == GLFW_RELEASE)
	{
		if(_currDragging != nullptr)
			updateCursor(CURSOR_TYPE_ARROW);
		_currDragging = nullptr;
		_startDraggingOffset = {0,0};
		_startDraggingCursorPos = {0,0};
	}
	if(draggable != nullptr)
	{
		if(action == GLFW_PRESS)
		{
			_currDragging = draggable;
			_startDraggingOffset = draggable->getWidgetToDrag()->getOffset();
			_startDraggingCursorPos = _cursorPos;
		}
	}
	return;

}

void GuiRender::onScroll(double xoffset, double yoffset)
{

}

void GuiRender::updateCursor(CursorType cursorType)
{
	if(cursorType != _cursorType)
	{
		_cursorType = cursorType;
		glfwDestroyCursor(_cursor);
		switch(_cursorType)
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

