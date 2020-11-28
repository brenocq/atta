//--------------------------------------------------
// GuiB
// column.cpp
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "column.h"

guib::Column::Column(ColumnInfo columnInfo):
	Widget({}), _children(columnInfo.children)
{
	Widget::setType("Column");
}

guib::Column::~Column()
{

}
