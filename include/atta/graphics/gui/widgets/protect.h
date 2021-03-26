//--------------------------------------------------
// GuiB
// protect.h
// Date: 2020-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_PROTECT_H
#define GUIB_PROTECT_H

#include <atta/graphics/gui/widgets/widget.h>

namespace guib {
	struct ProtectInfo {
		Widget* child;
	};

	class Protect : public Widget
	{
		public:
			Protect(ProtectInfo info);
			~Protect() override;// Override to not delete the child

			void preProcessSizeOffset() override;

			//---------- Getters and Setters ----------//
			Widget* getProtectedChild() const { return _protectedChild; }

		private:
			Widget* _protectedChild;
	};
}

#endif// GUIB_PROTECT_H
