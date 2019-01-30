
#include <SDL\SDL.h>
#include <Windows.h>

#include "Engine.h"
#include <iostream>

//Alternatively: Linker/Input/AdditionalDepdencies/..lib
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "SDL2main.lib")

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