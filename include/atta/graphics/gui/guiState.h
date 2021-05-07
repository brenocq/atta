//--------------------------------------------------
// Guib
// guiState.h
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_STATE_H
#define GUI_STATE_H
#include <vector>
#include <map>
#include <atta/graphics/gui/widgets/widgetStructs.h>
#include <atta/graphics/vulkan/texture.h>

namespace guib
{
	enum CursorType
	{
		CURSOR_TYPE_ARROW,
		CURSOR_TYPE_TEXT,
		CURSOR_TYPE_HAND,
		CURSOR_TYPE_CROSSHAIR,
		CURSOR_TYPE_HRESIZE,
		CURSOR_TYPE_VRESIZE
	};

	class FontLoader;
	class GuiRender;
	class Draggable;
	class Widget;

	namespace state
	{
		extern GuiRender* guiRender;
		extern Size screenSize;

		//---------- GuiB current ui state ----------//
		extern bool shouldClose;

		//---------- GuiB window/viewport handling ----------//
		extern guib::Offset cursorPos;
		extern CursorType cursorType;

		//---------- GuiB current drawing state ----------//
		extern float renderDepth;
		extern guib::Widget* focusedWidget;// Last one to render to apply right transparency 

		//---------- GuiB special widget handler ----------//
		// Handling ClickDetector
		extern std::vector<guib::ClickDetectorArea> clickableAreas;
		extern std::pair<guib::ClickDetectorArea, bool> currClickArea;// Current hovering/clicking

		// Handling Draggable
		extern std::vector<guib::Draggable*> draggables;
		extern guib::Draggable* currDragging;
		extern guib::Offset lastDraggingCursorPos;

		//---------- GuiB font render ----------//
		extern guib::FontLoader* fontLoader;

		//---------- Palette ----------//
		extern std::map<std::string, guib::Color> palette;

		//---------- Textures ----------//
		extern std::map<std::string, unsigned> textureIndex;
		extern std::vector<atta::vk::Texture*> textures;
	}
}

#endif// GUI_STATE_H
