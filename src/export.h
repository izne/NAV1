#ifndef NAV1_EXPORT_H
#define NAV1_EXPORT_H

#if defined(_WIN32) && defined(BUILDING_DLL)
  #define NAV1_EXPORT __declspec(dllexport)
#elif defined(_WIN32)
  #define NAV1_EXPORT __declspec(dllimport)
#elif defined(__BORLANDC__) && defined(BUILDING_DLL)
  #define NAV1_EXPORT __export
#elif defined(__GNUC__) && defined(BUILDING_DLL)
  #define NAV1_EXPORT __attribute__((visibility("default")))
#else
  #define NAV1_EXPORT
#endif

#endif
