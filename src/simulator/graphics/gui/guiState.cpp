//--------------------------------------------------
// Guib
// guiState.cpp
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "guiState.h"

namespace guib::state
{
	GuiRender* guiRender = nullptr;
	Size screenSize = {0,0, UNIT_SCREEN, UNIT_SCREEN};

	//---------- GuiB window/viewport handling ----------//
	Offset cursorPos = {0,0, UNIT_SCREEN, UNIT_SCREEN};
	CursorType cursorType = CURSOR_TYPE_ARROW;

	//---------- GuiB special widget handler ----------//
	// Handling ClickDetector
	std::vector<ClickDetectorArea> clickableAreas;
	std::pair<ClickDetectorArea, bool> currClickArea;// Current hovering/clicking
	// Handling Draggable
	std::vector<guib::Draggable*> draggables;
	Draggable* currDragging = nullptr;
	Offset lastDraggingCursorPos;

	//---------- GuiB font render ----------//
	FontLoader* fontLoader = nullptr;
}
