#include "engine/Engine.h"
#include <windows.h>
#include "WinUser.h"

int main( int argc, char* args[] )
{

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	Engine engine;
	engine.start();

	return 0;
}



