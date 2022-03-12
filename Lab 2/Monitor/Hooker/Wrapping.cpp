#include "Wrapping.h"

extern SharedData* sharedData;
extern LPVOID targetFunc;

HANDLE(WINAPI* real_CreateFileA) (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFileA;
HANDLE(WINAPI* real_CreateFileW) (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFileW;
HANDLE(WINAPI* real_FindFirstFileW) (LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData) = FindFirstFileW;
HANDLE(WINAPI* real_FindFirstFileA)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) = FindFirstFileA;
BOOL(WINAPI* real_FindNextFileW) (HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData) = FindNextFileW;
BOOL(WINAPI* real_FindNextFileA) (HANDLE hFindFile, LPWIN32_FIND_DATAA  lpFindFileData) = FindNextFileA;



HANDLE WINAPI
mine_CreateFileA(LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
) {
    printLogIn();

    HANDLE ret = INVALID_HANDLE_VALUE;
    if (strcmp(sharedData->hideFile, lpFileName)) {
        ret = real_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode,
            lpSecurityAttributes, dwCreationDisposition,
            dwFlagsAndAttributes, hTemplateFile);
    }

    printLogOut();

    return ret;
}


HANDLE WINAPI
mine_CreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) {
    printLogIn();


    HANDLE ret = INVALID_HANDLE_VALUE;

    if (wcscmp(sharedData->hideFileUnicode, lpFileName)) {
        ret = real_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
            lpSecurityAttributes, dwCreationDisposition,
            dwFlagsAndAttributes, hTemplateFile);
    }

    printLogOut();


    return ret;
}


HANDLE WINAPI
mine_FindFirstFileA(
    _In_ LPCSTR lpFileName,
    _Out_ LPWIN32_FIND_DATAA lpFindFileData
) {
    printLogIn();

    HANDLE ret = INVALID_HANDLE_VALUE;
    if (strcmp(sharedData->hideFile, lpFileName)) {
        ret = real_FindFirstFileA(lpFileName, lpFindFileData);
    }

    printLogOut();

    return ret;
}


HANDLE WINAPI
mine_FindFirstFileW(
    _In_ LPCWSTR lpFileName,
    _Out_ LPWIN32_FIND_DATAW lpFindFileData
) {
    printLogIn();

    HANDLE ret = INVALID_HANDLE_VALUE;
    if (wcscmp(sharedData->hideFileUnicode, lpFileName)) {
        ret = real_FindFirstFileW(lpFileName, lpFindFileData);
    }

    printLogOut();

    return ret;
}



BOOL WINAPI
mine_FindNextFileA(
    _In_ HANDLE hFindFile,
    _Out_ LPWIN32_FIND_DATAA lpFindFileData
) {
    printLogIn();
    BOOL ret = real_FindNextFileA(hFindFile, lpFindFileData);
    printLogOut();

    return ret;
}

BOOL WINAPI
mine_FindNextFileW(
    _In_ HANDLE hFindFile,
    _Out_ LPWIN32_FIND_DATAW lpFindFileData
) {

    printLogIn();
    BOOL ret = real_FindNextFileW(hFindFile, lpFindFileData);
    printLogOut();

    return ret;
}

__declspec(naked)
void funcWrapping_32() {

    __asm {
        push esp
        push ebp
        push eax
        push ebx
        push ecx
        push edx
        push edi
        push esi

        call printLogIn
        call printLogOut

        pop esi
        pop edi
        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp
        pop esp

        mov eax, targetFunc
        push eax
        ret
    }

}


void changeCall() {


    //MessageBoxA(0, sharedData->funcName, ("Log"), MB_ICONINFORMATION);
    //MessageBoxA(0, sharedData->hideFile, ("Log"), MB_ICONINFORMATION);


    if (strlen(sharedData->funcName) == 0) {
        return;
    }

    /* Find func-pointer in standart lib */
    targetFunc = DetourFindFunction("kernel32.dll", sharedData->funcName);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    /* Change call */
    if (!strcmp(sharedData->funcName, "CreateFileA")) {
        DetourAttach(&(PVOID&)real_CreateFileA, &mine_CreateFileA);
    }
    else if (!strcmp(sharedData->funcName, "CreateFileW")) {
        DetourAttach(&(PVOID&)real_CreateFileW, &mine_CreateFileW);
    }
    else if (!strcmp(sharedData->funcName, "FindFirstFileA")) {
        DetourAttach(&(PVOID&)real_FindFirstFileA, &mine_FindFirstFileA);
    }
    else if (!strcmp(sharedData->funcName, "FindFirstFileW")) {
        DetourAttach(&(PVOID&)real_FindFirstFileW, &mine_FindFirstFileW);
    }
    else if (!strcmp(sharedData->funcName, "FindNextFileA")) {
        DetourAttach(&(PVOID&)real_FindNextFileA, &mine_FindNextFileA);
    }
    else if (!strcmp(sharedData->funcName, "FindNextFileW")) {
        DetourAttach(&(PVOID&)real_FindNextFileW, &mine_FindNextFileW);
    }
    else {
        DetourAttach(&(PVOID&)targetFunc, &funcWrapping_32);
    }

    if (DetourTransactionCommit() != NO_ERROR) {
        debugMsg(L"Error commit");
    }



    //CHAR s[1024];
    //sprintf(s, "%s %s %d", sharedData->funcName, sharedData->hideFile ? sharedData->hideFile : "NULL", sharedData->semaphore);
    //MessageBoxA(0, s, ("Log"), MB_ICONINFORMATION);

}