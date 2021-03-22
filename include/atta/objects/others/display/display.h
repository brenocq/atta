//--------------------------------------------------
// Robot Simulator
// display.h
// Date: 2020-10-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_OTHERS_DISPLAY_DISPLAY_H
#define ATTA_OBJECTS_OTHERS_DISPLAY_DISPLAY_H

#include <string>
#include <vector>
#include <atta/objects/object.h>

namespace atta
{
	class Display : public Object
	{
		public:
			struct ScreenInfo
			{
				unsigned int width = 320;
				unsigned int height = 240;
			};

			struct CreateInfo
			{
				std::string name = "Box";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				vec3 scale = {1,1,1};
				float mass = 1.0f;
				vec3 color = {0.3f, 0.3f, 0.3f};
				ScreenInfo screenInfo = {320, 240};
			};

			Display(CreateInfo info);
			~Display();

			//---------- Getters ----------//
			ScreenInfo getDisplayInfo() const { return _info; }
			int getTextureIndex() const { return _textureIndex; }
			int getMaterialIndex() const { return _materialIndex; }
			std::vector<uint8_t> getBuffer() const { return _buffer; }

			//---------- Setters ----------//
			void setMaterialIndex(int index) { _materialIndex = index; }
			void setTextureIndex(int index) { _textureIndex = index; }
			void setBuffer(std::vector<uint8_t> buffer) { _buffer=buffer; }

		private:
			ScreenInfo _info;

			// Display buffer
			std::vector<uint8_t> _buffer;

			//----- Graphics -----//
			int _textureIndex;
			int _materialIndex;

	};
}

#endif// ATTA_OBJECTS_OTHERS_DISPLAY_DISPLAY_H
