//--------------------------------------------------
// Robot Simulator
// infinite.cpp
// Date: 2021-02-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "infinite.h"
#include "simulator/graphics/core/texture.h"
#include "stbImage.h"
#include "simulator/helpers/log.h"

namespace atta
{
	InfiniteLight::InfiniteLight(CreateInfo info):
		Object({info.name, info.position, info.rotation, {1,1,1}, 0}),
		_textureIndex(info.texture), _worldRadius(info.worldRadius)
	{
		Object::setType("InfiniteLight");
		_isLight = true;

		generateDistribution2DTexture();
	}

	InfiniteLight::~InfiniteLight()
	{
	}

	void InfiniteLight::generateDistribution2DTexture()
	{
		// The distribution texture is used to sample points in the environment texture 
		// according to the luminance of each pixels, pixels with higher luminance
		// are more likely to be selected when sampling the infinite light

		// The distribution texture:
		// The first column stores the marginal cumulative density function for each row in the image
		// Excuding the first column, each row stores the cumulative density function for each pixels in the row being selected

		//---------- Load image data to temporary buffer ----------//
		std::string filePath = 	"assets/textures/"+Texture::textureInfos[_textureIndex].fileName;

		if(filePath.find(".hdr") == std::string::npos)
		{
			Log::error("InfiniteLight", "Today only .hdr textures are supported to create infinite lights. ($0)", filePath);
			exit(1);
		}

		int channels;
		float* pixels = stbi_loadf(filePath.c_str(), &_width, &_height, &channels, 0);
		_distributionTexture.resize((_width+1)*_height);

		if(channels != 3)
		{
			Log::error("InfiniteLight", "Today only textures with 3 channels are supported. ($0)", filePath);
			exit(1);
		}

		//---------- Create temporary filtered luminance texture ----------//
		std::vector<float> img(_width*_height);
		for(int v=0; v<_height; ++v)
		{
			float vp = float(v)/_height;
			float sinTheta = std::sin(M_PI * float(v + .5f) / float(_height));
			for(int u=0; u<_width; ++u)
			{
				float up = float(u)/_width;
				img[u + v * _width] = luminance(filter(pixels, u, v));
				img[u + v * _width] *= sinTheta;
			}
		}
		stbi_image_free(pixels);
		//---------- Create 2D distribution texture from img ----------//
		// TODO create aux function to calculate 1D distribution returning integral
		//----- Create conditional sampling distributions -----//
		std::vector<float> rowIntegral(_width);
		for(int v=0; v<_height; v++)
		{
			// Compute integral of step function at xi
			float* distRow = &_distributionTexture[1+v*(_width+1)];
			float* imgRow = &img[v*_width];
			distRow[0] = 0;
			for(int u=1; u<_width; u++)
			{
				distRow[u] = distRow[u-1] + imgRow[u-1]/_width;
			}
			float rowInt = distRow[_width-1] + imgRow[_width-1]/_width;
			rowIntegral.push_back(rowInt);

			//  Transform step function integral to CDF
			if(rowInt==0)
			{
				// Equally distributed 
				for(int i=0; i<_width; i++)
				{
					distRow[i] = float(i)/_width;
				}
			}
			else
			{
				// Divide by integral to get CDF
				for(int i=0; i<_width; i++)
				{
					distRow[i]/=rowInt;
				}
			}
		}

		//----- Create marginal sampling distribution -----//
		// Compute setp function
		_distributionTexture[0] = 0;
		for(int v=1; v<_height; v++)
		{
			_distributionTexture[v*(_width+1)] = _distributionTexture[(v-1)*(_width+1)] + rowIntegral[v-1]/_height;
		}
		float colInt = _distributionTexture[(_height-1)*(_width+1)] + rowIntegral[_height-1]/_height;
		if(colInt==0)
		{
			// Equally distributed 
			for(int i=0; i<_height; i++)
			{
				_distributionTexture[i*(_width+1)] = float(i)/_height;
			}
		}
		else
		{
			// Divide by integral to get CDF
			for(int i=0; i<_width; i++)
			{
				_distributionTexture[i*(_width+1)] /= colInt;
			}
		}

		//----- Generate distribution access from distribution texture -----//
		_distributionAccess.resize(_width*_height*3);
		for(int v=0; v<_height; v++)
		{
			float vNorm = float(v)/_height;

			// Find row 
			int row = _height-1;
			float pdfRow = 0;
			for(int i=0; i<_height; i++)
			{
				float val = _distributionTexture[i*(_width+1)];
				if(i>0)
					pdfRow = val-_distributionTexture[(i-1)*(_width+1)];
				if(val>vNorm)
				{
					row = i;
					break;
				}
			}

			// Fow each possible column
			for(int u=0; u<_width; u++)
			{
				float uNorm = float(u)/_width;

				int col = _width-1;
				float pdfCol = 0;
				for(int i=1; i<_width; i++)
				{
					float val = _distributionTexture[row*(_width+1)+i];
					if(i>1)
						pdfRow = val-_distributionTexture[row*(_width+1)+(i-1)];
					if(val>vNorm)
					{
						col = i;
						break;
					}
				}

				_distributionAccess[(v*_width+u)*3] = float(col)/_width;
				_distributionAccess[(v*_width+u)*3+1] = float(row)/_height;
				_distributionAccess[(v*_width+u)*3+1] = pdfRow*pdfCol;
			}
		}

		//---------- Add texture and store index ----------//
		_pdfTextureIndex = Texture::fromBuffer(_distributionAccess.data(), _width, _height, Texture::TYPE_BUFFER_FLOAT_3);
	}

	vec3 InfiniteLight::filter(float* pixels, int u, int v)
	{
		vec3 p;

		int curr = (u+v*_width)*3;
		int bef = (((u-1)%_width)+v*_width)*3;
		int af = (((u+1)%_width)+v*_width)*3;

		p.x = 0.8*pixels[curr] + 0.1*pixels[bef] + 0.1*pixels[af];
		p.y = 0.8*pixels[curr+1] + 0.1*pixels[bef+1] + 0.1*pixels[af+1];
		p.z = 0.8*pixels[curr+2] + 0.1*pixels[bef+2] + 0.1*pixels[af+2];

		return p;
	}

	float InfiniteLight::luminance(vec3 rgb)
	{
		return 0.212671f*rgb.x + 0.715160f*rgb.y + 0.072169f*rgb.z;
	}
}
