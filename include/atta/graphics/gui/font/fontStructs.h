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

	// GLYPH INFO DESCRIPTION
	//
	//  |left						 |
	//  |<---->+---------+ ^         |
	//  |   ^  |         | |         |
	//  |   |  |         | |         |
	//  |top|  |         | |         |
	//  |   |  |  glyph  | | height  |
	//  |   v  |         | |         |
	//--o------|         | |---------o--...
	//  |      |         | |         |
	//  |      +---------+ v         |
	//  |      <--------->			 |
	//  |         width       advance|
	//  |--------------------------->|

	struct GlyphInfo
	{
		// Info about each glyph stored in the atlas [0,1]
		unsigned width=0;
		unsigned height=0;
		unsigned x=0;
		unsigned y=0;
		unsigned left=0;
		unsigned top=0;
		unsigned advance=0;
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
