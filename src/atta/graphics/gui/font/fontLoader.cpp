//--------------------------------------------------
// GuiB
// fontLoader.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/font/fontLoader.h>
#include <atta/helpers/log.h>
#include <iostream>

namespace guib {
	FontLoader::FontLoader(std::shared_ptr<atta::vk::Device> device, std::shared_ptr<atta::vk::CommandPool> commandPool, std::string filename):
		_filename(filename), _baseHeight(18),
		_device(device), _commandPool(commandPool)
	{
		FT_Error error;

		// Create instance of FreeType2 library
		error = FT_Init_FreeType(&_library);
		if(error)
		{
			Log::error("FontLoader", "Failed to init FreeType!");
		}

		// Create face (load font file)
		error = FT_New_Face(_library, filename.c_str(), 0, &_face);
		if(error == FT_Err_Unknown_File_Format)
		{
			Log::error("FontLoader", "This font format is unsupported! [w]"+filename);
		}
		else if (error)
		{
			Log::error("FontLoader", "Error when loading font file! [w]"+filename);
		}
		else
		{
			Log::success("FontLoader", "Font file \"$0\" loaded successfully.", filename);
			Log::success("FontLoader", "Qty glyphs: $0", _face->num_glyphs);
			//Log::success("FontLoader", "Flags: $0", _face->face_flags);
		}

		// Set character size
		error = FT_Set_Pixel_Sizes( _face,// handle to face object
                            0,  		 // pixel_width
                            _baseHeight);// pixel_height
		if(error)
		{
			Log::error("FontLoader", "Error when setting char size!");
		}

		// Create font texture
		_fontTexture.atlas.width = 1024;
		_fontTexture.atlas.height = 1024;
		_fontTexture.padding = 5;

		// Allocate font texture buffer
		unsigned size = _fontTexture.atlas.width*_fontTexture.atlas.height;
		_fontTexture.atlas.data = new unsigned char[size];
		for(int i=0; i<size; i++) 
			_fontTexture.atlas.data[i] = 0;

		loadGlyphs();
		//testFontTerminal("test");
		
		// Load texture
		_texture = std::make_shared<atta::vk::Texture>(
				_device, 
				_commandPool, 
				_fontTexture.atlas.data, 
				(VkExtent2D){_fontTexture.atlas.width, _fontTexture.atlas.height},
				atta::Texture::FORMAT_R_UBYTE);

		// Clear FreeType resources
		FT_Done_Face(_face);
		FT_Done_FreeType(_library);
	}

	FontLoader::~FontLoader()
	{
		delete _fontTexture.atlas.data;
	}

	void FontLoader::loadGlyphs()
	{
    	FT_Int32 flags = FT_LOAD_RENDER | FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
		FT_Error error;

		// Region in atlas to copy
		unsigned int currX = 0;
		unsigned int currY = 0;
		unsigned int maxY = 0;// Maximum y in the last line of caracters

		// Try to load all characters
		for(unsigned char i = 0; i<255; i++)
		{
			// Load current character
			int glyphIndex = FT_Get_Char_Index(_face, i);
			if(glyphIndex == 0)// Check if there is this character in the font
				continue;

			error = FT_Load_Glyph(_face, glyphIndex, flags);
			if(error)
			{
				Log::warning("FontLoader", "Error loading glyph: '"+std::to_string(i)+"'!");
				return;
			}

			// Get glyph info
			FT_GlyphSlot slot = _face->glyph;
			FT_Bitmap bitmap = slot->bitmap;
			int glyphTop  = slot->bitmap_top;
			int glyphLeft  = slot->bitmap_left;
			int glyphAdvance  = slot->advance.x;

			// Check possible atlas region
			if(currX+bitmap.width>=_fontTexture.atlas.width)
			{
				currX = 0;
				currY = maxY;

				if(currY+bitmap.rows>_fontTexture.atlas.height)
				{
					Log::error("FontLoader", "Not enough space to fill font atlas glyph!");
					exit(1);
				}
			}

			// Copy from bitmap to atlas
    		unsigned char *srcPtr = bitmap.buffer;
    		unsigned char *dstPtr = _fontTexture.atlas.data + (currY*_fontTexture.atlas.width + currX);
			for(int j=0;j<bitmap.rows;j++)
			{
				memcpy(dstPtr, srcPtr, bitmap.width);
				dstPtr += _fontTexture.atlas.width;
				srcPtr += bitmap.pitch;
			}

			// Save glyph info
			_fontTexture.glyphsInfo[i] = {
				.width = (float)bitmap.width,
				.height = (float)bitmap.rows,
				.x =	(float)currX,
				.y =	(float)currY,
				.left =	(float)glyphLeft,
				.top =	(float)glyphTop,
				.advance = (float)(glyphAdvance>>6),
			};
			//Log::debug("FontLoader", "Letter: $0 wh($1,$2), xy($3,$4) lt($5,$6) ad $7", char(i), 
			//		_fontTexture.glyphsInfo[i].width, _fontTexture.glyphsInfo[i].height, 
			//		_fontTexture.glyphsInfo[i].x, _fontTexture.glyphsInfo[i].y, 
			//		_fontTexture.glyphsInfo[i].left, _fontTexture.glyphsInfo[i].top, 
			//		_fontTexture.glyphsInfo[i].advance);

			// Move currX currY
			currX += bitmap.width+_fontTexture.padding;
			maxY = std::max(maxY, currY+bitmap.rows+_fontTexture.padding);

		}
	}

	void FontLoader::testFontTerminal(std::string text)
	{
		for(auto letter : text)
		{
			GlyphInfo gInfo = _fontTexture.glyphsInfo[letter];

			unsigned w = _fontTexture.atlas.width;
			unsigned h = _fontTexture.atlas.height;
    		unsigned char *ptr = _fontTexture.atlas.data+int(gInfo.x*w)+int(gInfo.y*h*w);
			for(int i=0; i<gInfo.height*h; i++)
			{
				for(int j=0; j<gInfo.width*w;j++)
				{
					char c = 'o';
					if(ptr[j]==0)
						c = ' ';
					else if(ptr[j]<=200)
						c = '*';
					std::cout << c;
				}
				ptr+=w;
				std::cout << std::endl;
			}
		}
	}
}
