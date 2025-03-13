#include <iostream>
#include "NinjaNameGenerator.h"
#include "DojoTraversal.h"
#include "NinjaMap.h"

int main()
{
	NinjaNameGenerator n("imre", "papp");
	std::cout << n.getNinjaName() << std::endl;

	DojoTraversal dojoTraversal;
	dojoTraversal.Traversal();

	return 0;
}