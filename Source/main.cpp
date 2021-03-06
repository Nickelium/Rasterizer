#include "SDL2/include/SDL.h"
#include <Windows.h>
#include <fstream>

#include "Engine.h"

//Alternatively: Linker/Input/AdditionalDepdencies/..lib
#pragma comment(lib, "SDL2/lib/x86/SDL2.lib")
#pragma comment(lib, "SDL2/lib/x86/SDL2main.lib")
#pragma comment(lib, "SDL2/lib/x86/SDL2test.lib")

void StartHeapControl();

int main(int argc, char** argv)
{
	StartHeapControl();
	{
		Engine(1280, 720).Run();
	}
	_CrtDumpMemoryLeaks();

	return 0;
}

void StartHeapControl()
{
	#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 156 );
	#endif
}