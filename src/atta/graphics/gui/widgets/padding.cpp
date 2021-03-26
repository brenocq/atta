//--------------------------------------------------
// GuiB
// padding.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/widgets/padding.h>
#include <atta/helpers/log.h>

namespace guib
{
	Padding::Padding(PaddingInfo paddingInfo):
		Widget({.child=paddingInfo.child}), _padding(paddingInfo.padding)
	{
		Widget::setType("Padding");

	}


	void Padding::startPreProcess()
	{
		Widget::startPreProcess();
		// Convert padding UNIT_PIXEL ro UNIT_SCREEN

		// Convert padding UNIT_PERCENT ro UNIT_SCREEN
	}

	void Padding::preProcessSizeOffset()
	{
		Log::warning("Padding", "Padding was not implemented yet!");
		Widget::fillParent();
		if(_child)
		{
			Size childSize;
			Offset childOffset;
			_child->parentAsksSizeOffset(childSize, childOffset);
			//else if(type=="Padding")
			//{
			//	guib::Padding* padding = (guib::Padding*)widget;
			//	guib::PaddingValues pad = padding->getPadding();
			//	guib::Widget* child = padding->getChild();
			//	if(child!=nullptr)
			//	{
			//		currOffset.x+=currSize.width*pad.left;
			//		currOffset.y+=currSize.height*pad.top;

			//		currSize.width-=(currSize.width*pad.left+currSize.width*pad.right);
			//		currSize.height-=(currSize.height*pad.top+currSize.height*pad.bottom);
			//		renderWidget(commandBuffer, currOffset, currSize, child);
			//	}
			//}

			//switch(_hAlignment)
			//{
			//	case guib::ALIGN_START:
			//		{
			//		}
			//		break;
			//	case guib::ALIGN_CENTER:
			//		{
			//			float offset = (_size.width-childSize.width)*0.5f;
			//			_offset.x = _offset.x+offset;
			//		}
			//		break;
			//	case guib::ALIGN_END:
			//		{
			//			float offset = _size.width-childSize.width;
			//			_offset.x = _offset.x+offset;
			//		}
			//		break;
			//}

			//switch(_vAlignment)
			//{
			//	case guib::ALIGN_START:
			//		{
			//		}
			//		break;
			//	case guib::ALIGN_CENTER:
			//		{
			//			float offset = (_size.height-childSize.height)*0.5f;
			//			_offset.y = _offset.y+offset;
			//		}
			//		break;
			//	case guib::ALIGN_END:
			//		{
			//			float offset = _size.height-childSize.height;
			//			_offset.y = _offset.y+offset;
			//		}
			//		break;
			//}

			_size = childSize;
		}
	}
}
