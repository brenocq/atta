//--------------------------------------------------
// Atta Project
// project.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
// Project template to create simulations to run on the Atta Simulator
#ifndef ATTA_PROJECT_H
#define ATTA_PROJECT_H
#include "atta.h"

namespace atta
{
	class Atta;
	class Project
	{
		public:
			Project();
			~Project();

			Atta::CreateInfo getAttaCreateInfo() { return _attaCreateInfo; }
		protected:
			Atta::CreateInfo _attaCreateInfo {};
	};
}

#endif// ATTA_PROJECT_H
