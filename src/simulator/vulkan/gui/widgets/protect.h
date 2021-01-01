//--------------------------------------------------
// GuiB
// protect.h
// Date: 2020-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_PROTECT_H
#define GUIB_PROTECT_H

#include "widget.h"

namespace guib {
	struct ProtectInfo {
		Widget* child;
	};

	class Protect : public Widget
	{
		public:
			Protect(ProtectInfo info);
			~Protect();

			void render();

			//---------- Getters and Setters ----------//
			Widget* getProtectedChild() const { return _protectedChild; }

		private:
			Widget* _protectedChild;
	};
}

#endif// GUIB_PROTECT_H
