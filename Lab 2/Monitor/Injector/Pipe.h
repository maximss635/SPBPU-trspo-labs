#ifndef PIPE_H
#define PIPE_H

#include <Windows.h>


struct SharedData {
	CHAR funcName[64];

	CHAR hideFile[MAX_PATH]; 
	WCHAR hideFileUnicode[MAX_PATH];

	/* Field bufferedLogs is shared sourse.
		So before write to bufferedLogs we must wait,
		while SEMAPHORE begin FALSE */
	BOOL semaphore;

	/* 256 - symbols in one log
		8 - capacity of buffered logs
		(the buffer is flushed regularly, so 32 is enough) */
	CHAR bufferedLogs[8][256];
	DWORD logsBufferedCnt;

	BOOL isEnd;
};

#endif // PIPE_H
