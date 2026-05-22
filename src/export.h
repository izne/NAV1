#ifndef NAV_EXPORT_H
#define NAV_EXPORT_H

#if defined(_WIN32) && defined(BUILDING_DLL)
  #define NAV_EXPORT __declspec(dllexport)
#elif defined(_WIN32)
  #define NAV_EXPORT __declspec(dllimport)
#elif defined(__BORLANDC__) && defined(BUILDING_DLL)
  #define NAV_EXPORT __export
#else
  #define NAV_EXPORT
#endif

#endif
