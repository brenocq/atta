//--------------------------------------------------
// GuiB
// column.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_COLUMN_H
#define GUIB_COLUMN_H

#include "widget.h"
#include <vector>

namespace guib {
	struct ColumnInfo {
		std::vector<Widget*> children = {};
	};

	class Column : public Widget
	{
		public:
			Column(ColumnInfo columnInfo);
			~Column();

			//---------- Getters and Setters ----------//
			std::vector<Widget*> getChildren() const { return _children; }

		private:
			std::vector<Widget*> _children;
	};
}

#endif// GUIB_COLUMN_H
