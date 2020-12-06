//--------------------------------------------------
// GuiB
// fontLoader.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_FONT_LOADER_H
#define GUIB_FONT_LOADER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <string>
#include "fontStructs.h"

namespace guib {
	class FontLoader
	{
		public:
			FontLoader(std::string filename);
			~FontLoader();

		private:
			std::string _filename;
			FT_Library _library;
			FT_Face    _face;
	};
}

#endif// GUIB_FONT_LOADER_H
