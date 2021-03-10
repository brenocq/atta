//--------------------------------------------------
// Robot Simulator
// infinite.h
// Date: 2021-02-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_INFINITE_LIGHT_H
#define ATTA_INFINITE_LIGHT_H

#include "simulator/objects/object.h"

namespace atta
{
	class InfiniteLight : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "InfiniteLight";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				int texture = -1;// Texture index
				int irradianceTexture = -1;// It is possible to  load an irradiance map texture to avoid precomputing
				float worldRadius = 5000;// World radius in meters
			};

			InfiniteLight(CreateInfo info);
			~InfiniteLight();

			//---------- Getters ----------//
			int getTextureIndex() const { return _textureIndex; }
			int getPdfTextureIndex() const { return _pdfTextureIndex; }
			int getIrradianceTextureIndex() const { return _irradianceTextureIndex; }
			vec3 getPrecomputedPower() const { return _precomputedPower; }
			float getWorldRadius() const { return _worldRadius; }

		private:
			void generateDistribution2DTexture();
			vec3 filter(float* pixels, int u, int v);
			float luminance(vec3 rgb);

			int _textureIndex;
			int _pdfTextureIndex;
			int _irradianceTextureIndex;
			vec3 _precomputedPower;
			float _worldRadius;
			int _width;
			int _height;
			std::vector<float> _distributionTexture;
			std::vector<float> _distributionAccess;
	};
}

#endif// ATTA_INFINITE_LIGHT_H
