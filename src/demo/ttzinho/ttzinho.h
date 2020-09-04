//--------------------------------------------------
// Robot Ttzinho
// ttzinho.h
// Date: 2020-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef TTZINHO_H
#define TTZINHO_H

#include <string>
#include <vector>
#include "simulator/object.h"

class Ttzinho
{
	public:
		Ttzinho();
		~Ttzinho();

		void run();
		Object* getObject() const { return _object; }

	private:
		Object* _object;
};

#endif// TTZINHO_H

