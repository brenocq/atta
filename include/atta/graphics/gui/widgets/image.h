//--------------------------------------------------
// GuiB
// image.h
// Date: 2021-05-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_IMAGE_H
#define GUIB_IMAGE_H

#include <atta/graphics/gui/widgets/widget.h>
#include <atta/graphics/gui/widgets/widgetStructs.h>

namespace guib {
	struct ImageInfo {
		std::string name = "";
		Color color = {1,1,1,1};
		BoxRadius radius = {.0,.0,.0,.0};
		Size size = {1,1};
		Offset offset = {0,0};
		Widget* child = nullptr;
	};

	class Image : public Widget
	{
		public:
			Image(ImageInfo info);

			void render() override;

			//---------- Getters and Setters ----------//
			void setColor(Color color) { _color=color; }
			Color getColor() const { return _color; }
			BoxRadius getRadius() const { return _radius; }

		private:
			Color _color;
			BoxRadius _radius;
			std::string _name;
	};
}

#endif// GUIB_IMAGE_H
