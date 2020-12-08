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
#include "simulator/vulkan/texture.h"
#include "simulator/vulkan/device.h"
#include "simulator/vulkan/commandPool.h"

namespace guib {
	class FontLoader
	{
		public:
			FontLoader(Device* device, CommandPool* commandPool, std::string filename);
			~FontLoader();

			//---------- Getters and Setters ----------//
			Texture* getTexture() const { return _texture; }
			FontTexture getFontTexture() const { return _fontTexture; }
		private:
			void loadGlyphs();
			void testFontTerminal(std::string text);

			std::string _filename;
			FT_Library _library;
			FT_Face    _face;

			FontTexture _fontTexture;

			// Vulkan specific
			Device* _device;
			CommandPool* _commandPool;
			Texture* _texture;
	};
}

#endif// GUIB_FONT_LOADER_H
