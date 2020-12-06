//--------------------------------------------------
// GuiB
// fontLoader.h
// Date: 2020-12-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "fontLoader.h"
#include "simulator/helpers/log.h"

namespace guib {
	FontLoader::FontLoader(std::string filename):
		_filename(filename)
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
			Log::error("FontLoader", "This font format is unsupported! "+filename);
		}
		else if (error)
		{
			Log::error("FontLoader", "Error when loading font file! "+filename);
		}
		else
		{
			Log::success("FontLoader", "Font file \""+filename+"\" loaded successfully.");
			Log::success("FontLoader", "Qty glyphs: "+std::to_string(_face->num_glyphs), false);
			Log::success("FontLoader", "Flags: "+std::to_string(_face->face_flags), false);
		}

		// Set character size
		error = FT_Set_Char_Size(
          _face,
          0,// 		Width equal height
          16*64,// 	Height of 16 pixels
          300,//	Horizontal device resolution
          300//		Vertical device resolution
		  );
		if(error)
		{
			Log::error("FontLoader", "Error when setting char size!");
		}

		Log::debug("FontLoader", "Glyph index of 'B': "+std::to_string(FT_Get_Char_Index(_face, 'B')));

	}

	FontLoader::~FontLoader()
	{

	}
}
