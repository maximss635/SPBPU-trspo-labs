#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

#ifndef MAIN_H
#define MAIN_H

#include <io.h>

#include "ArgParser.h"
#include "Injector.h"
#include "Pipe.h"

HANDLE hMapFile;
LPCWSTR pBuf;
LPCSTR dllPath = "Hooker.dll";

#endif // MAIN_H
