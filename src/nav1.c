#include <stdio.h>
#include <windows.h>
#include "nav1.h"
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

/* Auto-generated version info */
#define NAV1_FILE_DESCRIPTION "Angelov NAV1 System"
#define NAV1_ORIGINAL_FILENAME "NAV1.dll"

NAV1_EXPORT void Version(void)
{
    MessageBoxA(NULL, NAV1_FILE_DESCRIPTION, NAV1_ORIGINAL_FILENAME, MB_OK | MB_ICONINFORMATION);
}

NAV1_EXPORT void CALLBACK WinVer(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hwnd;
    (void)hinst;
    (void)lpszCmdLine;
    (void)nCmdShow;
    Version();
}
