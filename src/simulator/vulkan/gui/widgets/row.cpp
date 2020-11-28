//--------------------------------------------------
// GuiB
// row.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "row.h"

guib::Row::Row(RowInfo rowInfo):
	Widget({}), _children(rowInfo.children)
{
	Widget::setType("Row");
}

guib::Row::~Row()
{

}
