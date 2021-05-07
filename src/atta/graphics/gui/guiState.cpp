//--------------------------------------------------
// Guib
// guiState.cpp
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/guiState.h>

namespace guib::state
{
	GuiRender* guiRender = nullptr;
	Size screenSize = {0,0, UNIT_SCREEN, UNIT_SCREEN};

	//---------- GuiB current ui state ----------//
	bool shouldClose = false;

	//---------- GuiB window/viewport handling ----------//
	Offset cursorPos = {0,0, UNIT_SCREEN, UNIT_SCREEN};
	CursorType cursorType = CURSOR_TYPE_ARROW;

	//---------- GuiB current drawing state ----------//
	float renderDepth = 0.99;
	guib::Widget* focusedWidget = nullptr;// Last one to render to apply right transparency 

	//---------- GuiB special widget handler ----------//
	// Handling ClickDetector
	std::vector<ClickDetectorArea> clickableAreas;
	std::pair<ClickDetectorArea, bool> currClickArea;// Current hovering/clicking
	// Handling Draggable
	std::vector<guib::Draggable*> draggables;
	Draggable* currDragging = nullptr;
	Offset lastDraggingCursorPos;

	//---------- GuiB font render ----------//
	FontLoader* fontLoader;

	//---------- GuiB font colors ----------//
	std::map<std::string, guib::Color> palette {
		{"background", {0.125, 0.125, 0.125, 1.0}},
		{"lightDark", {0.25, 0.25, 0.25, 1.0}},
		{"light", {0.29, 0.29, 0.29, 1.0}},
		{"lightLight", {0.33, 0.33, 0.33, 1.0}},
		{"bright", {0.4, 0.4, 0.4, 1.0}},
		{"main", {0.89, 0.117, 0.117, 1.0}},
	};

	//---------- GuiB font icons ----------//
	// TODO release textures
	std::map<std::string, unsigned> textureIndex;
	std::vector<atta::vk::Texture*> textures;
}
