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

GuiRender::GuiRender(VkExtent2D imageExtent, GuiPipelineLayout* pipelineLayout):
	_imageExtent(imageExtent), _pipelineLayout(pipelineLayout), _cursorPos({0,0})
{
	guib::Widget::screenSize = {imageExtent.width, imageExtent.height};
}

GuiRender::~GuiRender()
{

}

void GuiRender::render(VkCommandBuffer commandBuffer, guib::Widget* root)
{
	guib::Offset currOffset = {0,0};
	guib::Size currSize = {1,1, guib::UNIT_PIXEL};
	_clickableAreas.clear();

	renderWidget(commandBuffer, currOffset, currSize, root);
}

void GuiRender::renderWidget(VkCommandBuffer commandBuffer, guib::Offset currOffset, guib::Size currSize, guib::Widget* widget)
{
	if(widget == nullptr)
		return;

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
	currSize*=size;

	std::string type = widget->getType();
	if(type=="Box")
	{
		guib::Box* box = (guib::Box*)widget;
		guib::Color color = box->getColor();

		GuiObjectInfo objectInfo;
		objectInfo.position = glm::vec2(currOffset.x, currOffset.y);
		objectInfo.size = glm::vec2(currSize.width, currSize.height);
		objectInfo.radius = 0.0;
		objectInfo.color = glm::vec4(color.r, color.g, color.b, color.a);

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
	else if(type=="Column")
	{
		guib::Column* column = (guib::Column*)widget;
		std::vector<guib::Widget*> children = column->getChildren();
		guib::Size totalSize = column->getChildrenTotalSize(currSize);

		for(auto& child : children)
		{
			renderWidget(commandBuffer, currOffset, currSize, child);

			guib::Size childSize = child->getSize();
			if(childSize.unitH == guib::UNIT_PIXEL)
			{
				childSize.height /= _imageExtent.height*currSize.height;
				childSize.unitH = guib::UNIT_PERCENT;
			}

			currOffset.y+=currSize.height*childSize.height;
		}
	}
	else if(type=="Row")
	{
		guib::Row* row = (guib::Row*)widget;
		std::vector<guib::Widget*> children = row->getChildren();
		guib::Size totalSize = row->getChildrenTotalSize(currSize);

		switch(row->getHAlignment())
		{
			case guib::ALIGN_LEFT:
				break;
			case guib::ALIGN_CENTER:
				{
					float offset = currSize.width/2-totalSize.width/2;
					currOffset.x += offset;
					currSize.width -= offset;
				}
				break;
			case guib::ALIGN_RIGHT:
				{
					std::cout << "curr:"<<currSize.width<<" size:"<<totalSize.width<<std::endl;
					float offset = totalSize.width;//currSize.width;//-
					currOffset.x += offset;
					currSize.width -= offset;
				}
				break;
		}

		for(auto& child : children)
		{
			renderWidget(commandBuffer, currOffset, currSize, child);

			guib::Size childSize = child->getSize();
			if(childSize.unitW == guib::UNIT_PIXEL)
			{
				childSize.width /= _imageExtent.width*currSize.width;
				childSize.unitW = guib::UNIT_PERCENT;
			}

			currOffset.x+=currSize.width*childSize.width;
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
			currSize/=child->getSize();
			renderWidget(commandBuffer, currOffset, currSize, child);
			padding->setSize(child->getSize());
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

		currSize/=child->getSize();
		renderWidget(commandBuffer, currOffset, currSize, child);
		clickDetector->setSize(child->getSize());
	}
}

void GuiRender::onKey(int key, int scancode, int action, int mods)
{
}

void GuiRender::onCursorPosition(double xpos, double ypos)
{
	_cursorPos.x = xpos/_imageExtent.width;
	_cursorPos.y = ypos/_imageExtent.height;
	//Log::debug("GuiRender", "cursorPos: "+_cursorPos.toString());
}

void GuiRender::onMouseButton(int button, int action, int mods)
{
	guib::ClickDetector* clickDetector = nullptr;
	for(int i=(int)_clickableAreas.size()-1; i>=0; i--)
	{
		guib::ClickDetectorArea clickArea = _clickableAreas[i];
		if(_cursorPos.x>=clickArea.offset.x &&
			_cursorPos.y>=clickArea.offset.y &&
			_cursorPos.x<=clickArea.offset.x+clickArea.size.width &&
			_cursorPos.y<=clickArea.offset.y+clickArea.size.height)
		{
			clickDetector = clickArea.clickDetector;
			break;
		}
	}

	if(clickDetector == nullptr)
		return;

	if(clickDetector->getOnClick())
	{
		if(action == GLFW_PRESS)
		{
			clickDetector->getOnClick()();
		}
	}
}

void GuiRender::onScroll(double xoffset, double yoffset)
{

}
