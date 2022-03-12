#include "Main.h"

void listenPipe(HANDLE hPipe, SharedData* sharedData) {


	printf("<Tracelogger>\n");

	while (!sharedData->isEnd) {
		
		if (sharedData->logsBufferedCnt != 0) {

			// Lock
			//printf("->");
			sharedData->semaphore = TRUE;

			// Print all buffered logs
			for (DWORD i = 0; i < sharedData->logsBufferedCnt; ++i) {
				printf("%s", sharedData->bufferedLogs[i]);
			}

			// Flush the buffer
			sharedData->logsBufferedCnt = 0;

			// Unlock
			//printf("<-");
			sharedData->semaphore = FALSE;
		}
	}

	printf("</Tracelogger>\n");

}

void writeParametrsToSharedMemory(LPCSTR funcName, LPCSTR hideFile) {
	LPCWSTR szName = L"pipe";
	LPCWSTR szMsg = L"abcd";
	
	SharedData sharedData;
	sharedData.logsBufferedCnt = 0;
	sharedData.isEnd = FALSE;
	sharedData.semaphore = FALSE;

	if (funcName)
		strcpy(sharedData.funcName, funcName);
	else
		strcpy(sharedData.funcName, "");

	if (hideFile)
		strcpy(sharedData.hideFile, hideFile);
	else
		strcpy(sharedData.hideFile, "");

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE, // use paging file 
		NULL, // default security 
		PAGE_READWRITE, // read/write access 
		0, // maximum object size (high-order DWORD) 
		1024, // maximum object size (low-order DWORD) 
		szName); // name of mapping object 

	if (hMapFile == NULL) {
		printf("Could not create file mapping object (%d).\n",
			GetLastError());
		return;
	}

	pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object 
		FILE_MAP_ALL_ACCESS, // read/write permission 
		0,
		0,
		sizeof(SharedData));

	if (pBuf == NULL) {
		printf("<Error>Could not map view of file (%d)</Error>\n",
			GetLastError());

		CloseHandle(hMapFile);

		return;
	}

	CopyMemory((PVOID)pBuf, &sharedData, sizeof(sharedData));

	return;
}

void deinit() {
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
}

/*	Usage:
		!Monitor.exe -pid 123 -func CreateFile
		!Monitor.exe -name explorer.exe -func CreateFile
		!Monitor.exe -name explorer.exe -hide "C:\hello.txt"
*/

int main(DWORD argc, LPCSTR argv[]) {

	ArgParser argParser(argc, argv);

	DWORD pid = argParser.getPid();

	LPCSTR processPath = argParser.getProcessName();
	LPCSTR funcName = argParser.getFunctionName();
	LPCSTR fileToHide = argParser.getFileToHide();

	printf("<Parametrs>\n");
	if (INVALID_PID == pid) {
		printf("\t<ProcessName>%s</ProcessName>\n", processPath);
	} else {
		printf("\t<ProcessPid>%d</ProcessPid>\n", pid);
	}
	printf("\t<FunctionName>%s</FunctionName>\n", funcName);
	if (fileToHide != NULL) {
		printf("\t<Hide>%s</Hide>\n", fileToHide);
	}
	printf("</Parametrs>\n");

	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	// Get full path to dll
	CHAR dllFullPath[MAX_PATH];
	wcstombs(dllFullPath, currentDir, MAX_PATH);
	strcat(dllFullPath, "\\");
	strcat(dllFullPath, dllPath);

	// Check dll-file exist
	if (-1 == _access(dllFullPath, 0)) {
		printf("<Error>No such dll: %s</Error>\n", dllFullPath);
		return 1;
	}
	
	Injector injector;

	/* Try to get process pid 
		(if it wasn't explicitly indicated in cmd args => pid = INVALID_PID)
		Wait, if there is no such process */
	if (INVALID_PID == pid) {
		while (TRUE) {
			pid = injector.getProcessByName(processPath);
			if (INVALID_PID != pid) {
				break;
			}

			printf("<Warning>No such active process: %s</Warning>\n", processPath);
			Sleep(1000);
		}
	}

	// Sending parametrs to dll
	writeParametrsToSharedMemory(funcName, fileToHide);

	// Start injecting
	HANDLE remoteThread = injector.inject(pid, dllFullPath);
	if (INVALID_HANDLE_VALUE != remoteThread) {
		//printf("[Success] Good injecting\n");
	} else {
		printf("<Error>Bad injecting</Error>\n");
	}

	WaitForSingleObject(remoteThread, NULL);
	listenPipe(hMapFile, (SharedData*)pBuf);

	deinit();

	return 0;
}