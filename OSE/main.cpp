
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <SFML/Graphics.hpp>
#include "Engine/StateManager.h"
#include "Engine/Utilities/MathUtils.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

int main()
{
	
	// Set the filepath on apple devices to resources within the .app file.
#ifdef __APPLE__    
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		// error!
	}
	CFRelease(resourcesURL);
	
	chdir(path);
	std::cout << "Current Path: " << path << std::endl;
#endif
	
	ig::SeedRandom();
	STATEMANAGER->Begin();
	STATEMANAGER->Cleanup();
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

