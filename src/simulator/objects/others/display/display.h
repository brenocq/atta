//--------------------------------------------------
// Robot Simulator
// display.h
// Date: 2020-10-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>
#include "simulator/object.h"

class Display : public Object
{
	public:
		struct DisplayInfo
		{
			unsigned int width = 320;
			unsigned int height = 240;
		};

		Display(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 scale = {0,0,0}, DisplayInfo info = {320, 240});
		~Display();

		//---------- Getters and Setters ----------//
		DisplayInfo getDisplayInfo() const { return _info; }
		void setTextureIndex(int index) { _textureIndex = index; }
		int getTextureIndex() const { return _textureIndex; }
		void setMaterialIndex(int index) { _materialIndex = index; }
		int getMaterialIndex() const { return _materialIndex; }
		std::vector<uint8_t> getBuffer() const { return _buffer; }
		void setBuffer(std::vector<uint8_t> buffer) { _buffer=buffer; }

	private:

		// Camera parameters
		DisplayInfo _info;

		// Texture index
		int _textureIndex;
		int _materialIndex;

		// Display buffer
		std::vector<uint8_t> _buffer;
};

#endif// DISPLAY_H

