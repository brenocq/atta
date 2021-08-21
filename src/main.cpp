#include "atta/core/assert.h"
#include "atta/core/log.h"

int main(int argc, char *argv[])
{
	if(argc == 1)
	{

	}
	else
	{
		if(std::string(argv[1]) == "--version")
		{
			std::cout << "Atta Simulator 0.0.2.0" <<
				"\nThis project is licensed under the MIT License\n";
		}
	}

	int x = 1;
	ASSERT(x == 1);
	ASSERT(x == 1, "Testing");
	LOG_VERBOSE("main", "T[g]e[r]s[]ting: $0", 1);
	LOG_DEBUG("main", "T[g]e[r]s[]ting: $0", 1);
	LOG_INFO("main", "T[g]e[r]s[]ting: $0", 1);
	LOG_WARN("main", "T[g]e[r]s[]ting: $0", 1);
	LOG_ERROR("main", "T[g]e[r]s[]ting: $0", 1);

	return EXIT_SUCCESS;
}
