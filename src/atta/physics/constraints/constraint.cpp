//--------------------------------------------------
// Atta Physics
// constraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/constraints/constraint.h>

namespace atta::phy
{
	Constraint::Constraint():
		_objA(nullptr), _objB(nullptr)
	{
	}

	Constraint::~Constraint()
	{

	}
}
