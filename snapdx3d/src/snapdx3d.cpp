// snapdx3d.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

DWORD WINAPI MainLoop(LPVOID) {
	// Тут запускаем наш event loop
	::MessageBoxA(NULL, "Hello from snapdx3d.dll", "", MB_OK);
	while (1)
	{
		Sleep(1000);
	}
	return 0L;
}