//--------------------------------------------------
// Atta Drawer
// drawer.h
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_DRAWER_H
#define ATTA_DRAWER_H

#include <vector>
#include "simulator/math/math.h"

namespace atta
{
	// Draw 3d objects mainly for debugging
	class Drawer
	{
		public:
			struct Line
			{
				pnt3 p0; 
				pnt3 p1; 
				vec3 c0 = vec3(1,0,0);
				vec3 c1 = vec3(1,0,0);
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
			static void clear() { get().clearImpl(); }

			// Get data to render
			static std::vector<Line>& getLines() { return get().getLinesImpl(); }
			static unsigned getMaxNumberOfLines() { return get().getMaxNumberOfLinesImpl(); }
			static unsigned getCurrNumberOfLines() { return get().getCurrNumberOfLinesImpl(); }

		private:
			Drawer(): 
				_maxNumberOfLines(10000),
				_currNumberOfLines(0)
			{
				_lines.resize(_maxNumberOfLines);
			};

			void addLineImpl(Line line);
			std::vector<Line>& getLinesImpl();
			void clearImpl();
			unsigned getMaxNumberOfLinesImpl() { return _maxNumberOfLines; }
			unsigned getCurrNumberOfLinesImpl() { return _currNumberOfLines; }

			std::vector<Line> _lines;
			unsigned _maxNumberOfLines;
			unsigned _currNumberOfLines;
	};
}
#endif// ATTA_DRAWER_H
