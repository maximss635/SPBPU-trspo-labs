#include "Injector.h"


Injector::Injector() {

}

Injector::~Injector() {

}


HANDLE Injector::inject(DWORD pid, LPCSTR dllFullPath) {

	// Open a handle to target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	// Allocate memory for the dllpath in the target process
	// length of the path string + null terminator
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(dllFullPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	// Write the path to the address of the memory we just allocated
	// in the target process
	WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllFullPath,
		strlen(dllFullPath) + 1, 0);

	// Create a Remote Thread in the target process which
	// calls LoadLibraryA as our dllpath as an argument -> program loads our dll
	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);

	// Wait for the execution of our loader thread to finish
	// WaitForSingleObject(hLoadThread, INFINITE);

	// Free the memory allocated for our dll path
	VirtualFreeEx(hProcess, pDllPath, strlen(dllFullPath) + 1, MEM_RELEASE);

	return hLoadThread;
}

DWORD Injector::getProcessByName(LPCSTR processName) {
	// Convert to unicode
	WCHAR processNameUnicode[MAX_PATH];
	mbstowcs(processNameUnicode, processName, strlen(processName) + 1);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 processEntry;
	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);

	DWORD pid;

	if (Process32First(snapshot, &processEntry)) {
		while (Process32Next(snapshot, &processEntry)) {
			if (!wcscmp(processEntry.szExeFile, processNameUnicode)) {
				pid = processEntry.th32ProcessID;
				CloseHandle(snapshot);

				return pid;
			}
		}
	}

	CloseHandle(snapshot);

	return INVALID_PID;
}