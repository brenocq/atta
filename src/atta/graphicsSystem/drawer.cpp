//--------------------------------------------------
// Atta Graphics System
// drawer.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/drawer.h>

namespace atta
{
	Drawer::Drawer(): 
		_maxNumberOfLines(100000),
		_currNumberOfLines(0),
		_linesChanged(false),
		_maxNumberOfPoints(1000000),
		_currNumberOfPoints(0),
		_pointsChanged(false)
	{
		_lines.resize(_maxNumberOfLines);
		_points.resize(_maxNumberOfPoints);
		glPointSize(5.0f);
		glLineWidth(2.0f);

		// Line VAO
		glGenVertexArrays(1, &_lineVAO);
        glGenBuffers(1, &_lineVBO);
        glBindVertexArray(_lineVAO);

        glBindBuffer(GL_ARRAY_BUFFER, _lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Line) * _maxNumberOfLines, _lines.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

		// Point VAO
		glGenVertexArrays(1, &_pointVAO);
        glGenBuffers(1, &_pointVBO);
        glBindVertexArray(_pointVAO);

        glBindBuffer(GL_ARRAY_BUFFER, _pointVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * _points.size(), _points.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	};

	Drawer& Drawer::getInstance()
	{
		static Drawer drawer;
		return drawer;
	}

	void Drawer::clear() { getInstance().clearImpl(); }
	void Drawer::clearImpl()
	{
		_currNumberOfLines = 0;
		_currNumberOfPoints = 0;
		_linesChanged = false;
		_pointsChanged = false;
	}
}
