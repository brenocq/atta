#include <iostream>
#include <atta/atta.h>

int main()
{
	atta::Atta::CreateInfo attaInfo {
		.createWindow=true,
	};
	atta::Atta atta(attaInfo);
	std::cout << "Working.... ok\n";
	return 0;
}
