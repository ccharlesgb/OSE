
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "SFML/Graphics.hpp"
#include "StateManager.h"
#include "MathUtils.h"

int main()
{
	ig::SeedRandom();
	STATEMANAGER->Begin();
	STATEMANAGER->Cleanup();
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}