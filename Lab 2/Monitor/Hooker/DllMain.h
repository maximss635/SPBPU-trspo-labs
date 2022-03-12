#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

#ifndef DLL_MAIN_H
#define DLL_MAIN_H

#include "Wrapping.h"

WCHAR pathDllDirectory[MAX_PATH];
WCHAR pipeName[MAX_PATH] = L"pipe";
HANDLE pipeHandle;
DWORD mode;
HANDLE hMapFile;
SharedData* sharedData;

void debugMsg(LPCTSTR msg);
BOOL readParametrsFromSharedMemory();
BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD callEvent, LPVOID reserved);

#endif // DLL_MAIN_H
