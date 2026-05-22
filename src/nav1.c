#include <stdio.h>
#include <windows.h>
#include "nav1.h"
#include "../NAV1_private.h"
#include "export.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    (void)hinstDLL;
    (void)lpvReserved;
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}


NAV1_EXPORT void Version(void)
{
    char buf[128];
    sprintf(buf, "%s\nVersion %s", FILE_DESCRIPTION, VER_STRING);
    MessageBoxA(NULL, TEXT(buf), TEXT(ORIGINAL_FILENAME), MB_OK | MB_ICONINFORMATION);
}

NAV1_EXPORT void CALLBACK WinVer(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hwnd;
    (void)hinst;
    (void)lpszCmdLine;
    (void)nCmdShow;
    Version();
}
