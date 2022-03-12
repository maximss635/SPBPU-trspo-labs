#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

#ifndef INJECTOR_H
#define INJECTOR_H

#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>

#define INVALID_PID 0

class Injector {
public:

	Injector();
	~Injector();

	HANDLE inject(DWORD pid, LPCSTR dllFullpath);

	DWORD getProcessByName(LPCSTR processName);
};

#endif // INJECTOR_H
