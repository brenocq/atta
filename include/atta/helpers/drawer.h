//--------------------------------------------------
// Atta Drawer
// drawer.h
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_HELPERS_DRAWER_H
#define ATTA_HELPERS_DRAWER_H

#include <vector>
#include <memory>
#include <atta/math/math.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/graphics/vulkan/commandPool.h>

namespace atta
{
	// Draw 3d objects mainly for debugging and visualization
	class Drawer
	{
		public:
			struct Line
			{
				alignas(16) vec4 p0; 
				alignas(16) vec4 p1; 
				alignas(16) vec4 c0 = vec4(1,0,1,1);
				alignas(16) vec4 c1 = vec4(1,0,1,1);
			};

			struct Point
			{
				alignas(16) vec4 p; 
				alignas(16) vec4 c = vec4(1,0,1,1);
			};

			Drawer(const Drawer&) = delete;
        	void operator=(const Drawer&) = delete;

			static Drawer& get()
			{
				static Drawer drawer;
				return drawer;
			}

			// Drawer for debugging
			static void addLine(Line line) { get().addLineImpl(line); }
			static void addPoint(Point point) { get().addPointImpl(point); }
			static void clear() { get().clearImpl(); }

			// Get line data to render
			static std::vector<Line>& getLines() { return get().getLinesImpl(); }
			static unsigned getMaxNumberOfLines() { return get().getMaxNumberOfLinesImpl(); }
			static unsigned getCurrNumberOfLines() { return get().getCurrNumberOfLinesImpl(); }
			static unsigned getCurrNumberOfLinesMemory() { return get().getCurrNumberOfLinesMemoryImpl(); }

			// Get point data to render
			static std::vector<Point>& getPoints() { return get().getPointsImpl(); }
			static unsigned getMaxNumberOfPoints() { return get().getMaxNumberOfPointsImpl(); }
			static unsigned getCurrNumberOfPoints() { return get().getCurrNumberOfPointsImpl(); }
			static unsigned getCurrNumberOfPointsMemory() { return get().getCurrNumberOfPointsMemoryImpl(); }

			// Update buffer memory
			static void updateBufferMemory(std::shared_ptr<vk::VulkanCore> vkCore, std::shared_ptr<vk::CommandPool> commandPool) { return get().updateBufferMemoryImpl(vkCore, commandPool); }

		private:
			Drawer(): 
				_maxNumberOfLines(100000),
				_maxNumberOfPoints(1000000),
				_currNumberOfLines(0),
				_currNumberOfLinesMemory(0),
				_currNumberOfPoints(0),
				_currNumberOfPointsMemory(0)
			{
				_lines.resize(_maxNumberOfLines);
				_points.resize(_maxNumberOfPoints);
			};

			// Core impl
			void clearImpl();
			void updateBufferMemoryImpl(std::shared_ptr<vk::VulkanCore> vkCore, std::shared_ptr<vk::CommandPool> commandPool);

			// Line impl
			void addLineImpl(Line line);
			std::vector<Line>& getLinesImpl() { return _lines; }
			unsigned getMaxNumberOfLinesImpl() { return _maxNumberOfLines; }
			unsigned getCurrNumberOfLinesImpl() { return _currNumberOfLines; }
			unsigned getCurrNumberOfLinesMemoryImpl() { return _currNumberOfLinesMemory; }

			// Point impl
			void addPointImpl(Point point);
			std::vector<Point>& getPointsImpl() { return _points; }
			unsigned getMaxNumberOfPointsImpl() { return _maxNumberOfPoints; }
			unsigned getCurrNumberOfPointsImpl() { return _currNumberOfPoints; }
			unsigned getCurrNumberOfPointsMemoryImpl() { return _currNumberOfPointsMemory; }

			std::vector<Line> _lines;
			unsigned _maxNumberOfLines;
			unsigned _currNumberOfLines;
			unsigned _currNumberOfLinesMemory;
			std::vector<Point> _points;
			unsigned _maxNumberOfPoints;
			unsigned _currNumberOfPoints;
			unsigned _currNumberOfPointsMemory;
	};
}
#endif// ATTA_HELPERS_DRAWER_H
