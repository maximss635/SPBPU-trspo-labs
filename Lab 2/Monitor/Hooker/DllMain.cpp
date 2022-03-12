#include "DllMain.h"

LPVOID targetFunc;

void printLogIn() {
    printLog(TRUE);
}

void printLogOut() {
    printLog(FALSE);
}

void printLog(BOOL in) {

    static int itteration = 1;

    time_t seconds = time(nullptr);
    LPSTR timeStr = asctime(localtime(&seconds));
    timeStr[strlen(timeStr) - 1] = 0;

    CHAR out[256];
    sprintf(out, "\t<Tracelog>\n"
        "\t\t<Itteration>%d</Itteration>\n"
        "\t\t<Function>%s</Function>\n"
        "\t\t<Time>%s</Time>\n"
        "\t\t<Action>%s</Action>\n"
        "\t</Tracelog>\n",
        itteration++,
        sharedData->funcName,
        timeStr,
        in ? "In" : "Out");

    // Wait
    while (sharedData->semaphore) {
        //debugMsg(L"wait");
    }


    if (sharedData->logsBufferedCnt > 8) {
        debugMsg(L"ASSERT");
    }

    // Add log to shared buffer
    strcpy(sharedData->bufferedLogs[sharedData->logsBufferedCnt++], out);

}

void debugMsg(LPCTSTR msg) {
    MessageBox(0, msg, TEXT("Log"), MB_ICONINFORMATION);
}

BOOL readParametrsFromSharedMemory() {
    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,            // read/write access 
        FALSE,                          // do not inherit the name 
        pipeName);                      // name of mapping object 

    if (hMapFile == NULL) {

        debugMsg(L"Error opening pipe");
        return FALSE;
    }

    sharedData = (SharedData*)MapViewOfFile(
        hMapFile,                       // handle to map object 
        FILE_MAP_ALL_ACCESS,            // read/write permission 
        0, 0, sizeof(SharedData));

    if (sharedData == NULL) {
        debugMsg(L"Could not map view of file \n");

        CloseHandle(hMapFile);

        return FALSE;
    }

     //UnmapViewOfFile(sharedData);
    CloseHandle(hMapFile);

    mbstowcs(sharedData->hideFileUnicode, 
        sharedData->hideFile, 
        strlen(sharedData->hideFile));

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD callEvent, LPVOID reserved) {
    
    if (DLL_PROCESS_DETACH == callEvent) {
        sharedData->isEnd = TRUE;
    }

	if (DLL_THREAD_DETACH != callEvent) {
		return TRUE;
	}
    
    //getPathDllDirectory();

    if (FALSE == readParametrsFromSharedMemory()) {
        return FALSE;
    }

    changeCall();

}
