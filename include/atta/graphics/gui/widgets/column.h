//--------------------------------------------------
// GuiB
// column.h
// Date: 2020-11-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_COLUMN_H
#define GUIB_COLUMN_H

#include <atta/graphics/gui/widgets/widget.h>
#include <vector>

namespace guib {
	struct ColumnInfo {
		Alignment hAlignment = ALIGN_START;
		Alignment vAlignment = ALIGN_START;
		std::vector<Widget*> children = {};
	};

	class Column : public Widget
	{
		public:
			Column(ColumnInfo columnInfo);
			~Column() override;// Delete children

			void preProcessSizeOffset() override;
			void startPreProcess() override;
			void endPreProcess() override;

			void render() override;

			void addOffsetTree(Offset offset) override;

			//---------- Getters and Setters ----------//
			std::vector<Widget*> getChildren() const { return _children; }
			Size getChildrenTotalSize();
			Alignment getHAlignment() const { return _hAlignment; }
			Alignment getVAlignment() const { return _vAlignment; }

		private:
			Alignment _hAlignment;
			Alignment _vAlignment;
			std::vector<Widget*> _children;
	};
}

#endif// GUIB_COLUMN_H
