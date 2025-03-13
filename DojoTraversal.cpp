#include "DojoTraversal.h"

void DojoTraversal::Traversal()
{
	NinjaMap ninjaMap("maps//18_in");
	
	std::cout << ninjaMap << std::endl;

	while (!ninjaMap.IsHolySymboleDestroyed())
	{
		ninjaMap.readNextMove();
	}
}
