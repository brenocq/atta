#ifndef CORNELL_BOX_H
#define CORNELL_BOX_H

#include "material.h"
#include "vertex.h"
#include <vector>

class CornellBox
{
public:
	static void create(
		float scale,
		std::vector<Vertex>& vertices,
		std::vector<uint32_t>& indices,
		std::vector<Material>& materials);
};

#endif// CORNELL_BOX_H
