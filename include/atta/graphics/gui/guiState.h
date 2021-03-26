//--------------------------------------------------
// Guib
// guiState.h
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_STATE_H
#define GUI_STATE_H
#include <vector>
#include <atta/graphics/gui/widgets/widgetStructs.h>

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

	namespace state
	{
		extern GuiRender* guiRender;
		extern Size screenSize;

		//---------- GuiB window/viewport handling ----------//
		extern guib::Offset cursorPos;
		extern CursorType cursorType;

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
	}
}

#endif// GUI_STATE_H
