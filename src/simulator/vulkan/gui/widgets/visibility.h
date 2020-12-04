//--------------------------------------------------
// GuiB
// visibility.h
// Date: 2020-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_VISIBILITY_H
#define GUIB_VISIBILITY_H

#include "widget.h"

namespace guib {
	struct VisibilityInfo {
		bool visible = true;
		Widget* child;
	};

	class Visibility : public Widget
	{
		public:
			Visibility(VisibilityInfo info);
			~Visibility();

			//---------- Getters and Setters ----------//
			void setVisible(bool visible) { _visible=visible; }
			bool getVisible() const { return _visible; }

		private:
			bool _visible;
	};
}

#endif// GUIB_VISIBILITY_H
