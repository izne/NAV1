#include <stdio.h>
#include "nav1.h"
#include "../NAV1_private.h"

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

NAV_EXPORT void Version()
{
    char buf[128];
    sprintf(buf, "%s\nVersion %s", FILE_DESCRIPTION, VER_STRING);
    MessageBoxA(NULL, TEXT(buf), TEXT(ORIGINAL_FILENAME), MB_OK | MB_ICONINFORMATION);
}

NAV_EXPORT void CALLBACK WinVer(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hwnd;
    (void)hinst;
    (void)lpszCmdLine;
    (void)nCmdShow;
    Version();
}
