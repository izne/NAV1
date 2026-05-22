#include "nav1.h"

#define VER "1.2"
#define VERDATE "May 2026"
#define VERSION "Angelov NAV1 System\nVersion " VER ", " VERDATE

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
    MessageBox(NULL, TEXT(VERSION), TEXT("NAV1.dll"), MB_OK | MB_ICONINFORMATION);
}

NAV_EXPORT void CALLBACK WinVer(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hwnd;
    (void)hinst;
    (void)lpszCmdLine;
    (void)nCmdShow;
    Version();
}
