//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// cleaningRobots2D.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EXAMPLE_CLEANING_ROBOTS_2D_UTILS
#define ATTA_EXAMPLE_CLEANING_ROBOTS_2D_UTILS
#include "simulator/math/vector.h"

namespace example
{
	void drawFilledCircle(std::vector<uint8_t>& floorData, float floorSize, int xc, int yc, int x, int y, int r)
	{
		for(int i=-x; i<=x; i++)
		{
			float newData = sqrt(i*i + y*y)/r;
			newData *= newData*255;
			//newData *= 255;
			floorData[(xc+i)+(yc+y)*floorSize] = (uint8_t)std::min((float)floorData[(xc+i)+(yc+y)*floorSize], newData);
		}
		for(int i=-x; i<=x; i++)
		{
			float newData = sqrt(i*i + y*y)/r;
			newData *= newData*255;
			//newData *= 255;
			floorData[(xc+i)+(yc-y)*floorSize] = (uint8_t)std::min((float)floorData[(xc+i)+(yc-y)*floorSize], newData);
		}
		for(int i=-y; i<=y; i++)
		{
			float newData = sqrt(i*i + x*x)/r;
			newData *= newData*255;
			//newData *= 255;
			floorData[(xc+i)+(yc+x)*floorSize] = (uint8_t)std::min((float)floorData[(xc+i)+(yc+x)*floorSize], newData);
		}
		for(int i=-y; i<=y; i++)
		{
			float newData = sqrt(i*i + x*x)/r;
			newData *= newData*255;
			//newData *= 255;
			floorData[(xc+i)+(yc-x)*floorSize] = (uint8_t)std::min((float)floorData[(xc+i)+(yc-x)*floorSize], newData);
		}
	}

	void cleanSolidCircle(std::vector<uint8_t>& floorData, float floorSize, int xc, int yc, int r)
	{
		if(xc<r || xc>floorSize-r || yc<r || yc>floorSize-r)
			return;
		// Draw filled circle using Bresenham's algorithm
		int x = 0, y = r;
		int d = 3 - 2 * r;
		drawFilledCircle(floorData, floorSize, xc, yc, x, y, r);
		while(y >= x)
		{
			// For each pixel we will draw all eight pixels
			x++;
			// Check for decision parameter and correspondingly update d, x, y
			if(d > 0)
			{
				y--; 
				d = d + 4 * (x - y) + 10;
			}
			else
				d = d + 4 * x + 6;
			drawFilledCircle(floorData, floorSize, xc, yc, x, y, r);
		}	
	}
}
#endif// ATTA_EXAMPLE_CLEANING_ROBOTS_2D_UTILS
