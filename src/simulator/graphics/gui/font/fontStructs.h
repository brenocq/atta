//--------------------------------------------------
// GuiB
// fontStructs.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUIB_FONT_STRUCTS_H
#define GUIB_FONT_STRUCTS_H

namespace guib {

	struct FontAtlas
	{
		// Matrix to store glyphs bitmaps (GPU font texture data)
		unsigned int width=0;
		unsigned int height=0;
    	unsigned char* data;
	};

	struct GlyphInfo
	{
		// Info about each glyph stored in the atlas [0,1]
		float width=0;
		float height=0;
		float x=0;
		float y=0;
		float left=0;
		float top=0;
		float advance=0;
	};


	struct FontTexture
	{
		// Atlas image texture and info about the font and glyphs
		FontAtlas atlas;
		std::vector<GlyphInfo> glyphsInfo = std::vector<GlyphInfo>(255);
		int padding = 3;
	};

}

#endif// GUIB_FONT_STRUCTS_H
