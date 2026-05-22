# NAV1 — Navigation Math Library

Windows DLL (C, 64-bit) providing geodesy, unit conversion, aviation, GPS/NMEA, flight management, and control-system math for autopilot and navigation applications.

## Quick Start

```batch
test.bat          — compile test harness + run all tests
```

Output: `=== Results: 117/117 passed ===`

## Build

| Tool            | Detail                                    |
|-----------------|-------------------------------------------|
| IDE             | Embarcadero Dev-C++ 6.3                   |
| Compiler        | TDM-GCC 9.2.0 64-bit (gcc, C, not C++)   |
| Project file    | `NAV1.dev`                                |
| CLI build       | `make -f Makefile.win`                    |
| Defines         | `BUILDING_DLL=1`, `_USE_MATH_DEFINES`     |
| Output          | `NAV1.dll` + `libNAV1.a` (import lib)    |

## Module Reference

| Module         | Prefix       | File(s)               | # Functions | Description                        |
|----------------|--------------|-----------------------|:-----------:|------------------------------------|
| Core / Geodesy | `NAV_SUP_`   | `dllmain.c` / `dll.h` | 8           | Distance, bearing, destination, midpoint, cross-track, DMS |
| Conversions    | `NAV_CONV_`  | `conv.c` / `conv.h`   | 20          | Length, speed, temperature, pressure |
| Navigation     | `NAV_SUP_`   | `nav.c` / `nav.h`     | 6           | Along-track, waypoints, rhumb line, antipode, compass |
| Aviation       | `NAV_SUP_`   | `aviation.c` / `aviation.h` | 5      | Wind correction, ISA atmosphere, pressure/density altitude |
| GPS            | `NAV_SUP_`   | `gps.c` / `gps.h`     | 6           | DMM conversion, GPS speed, NMEA checksum, lat/lon validation |
| Flight Mgmt    | `NAV_SUP_`   | `flight_mgmt.c` / `flight_mgmt.h` | 6 | Time to altitude, VS, TOD, waypoint, fuel |
| Control        | `NAV_SUP_`   | `control.c` / `control.h` | 10 (+ 5 structs) | LPF, slew limiter, moving avg, PID, complementary filter |
| NMEA           | `NAV_SUP_`   | `nmea.c` / `nmea.h`   | 1 (+ 1 struct) | Parse $--GGA / $--RMC sentences |

## NMEA Parser

Parses NMEA 0183 `$--GGA` and `$--RMC` sentences from any talker (GP, GN, GL, GA).

```c
NAV_Sup_NMEAData nd;
if (NAV_SUP_nmeaParse("$GPGGA,...*47", &nd)) {
    printf("lat=%.5f lon=%.5f alt=%.1f\n",
           nd.latitude, nd.longitude, nd.altitudeM);
}
```

## Test System

- `test_nav1.c` — standalone harness, links against `libNAV1.a`
- Macros: `TEST_NEAR`, `TEST_TRUE`, `TEST_STR`
- Exit code 0 = all pass, 1 = any failure
- Coverage: every exported function, edge cases included

## Portability

`export.h` provides the `NAV_EXPORT` macro:

| Build context          | `NAV_EXPORT` resolves to     |
|------------------------|------------------------------|
| Windows DLL build      | `__declspec(dllexport)`      |
| Windows consumer       | `__declspec(dllimport)`      |
| Borland C++ DLL build  | `__export`                   |
| Static library / DOS   | *(empty — no decoration)*    |

The math itself is pure C89 (sin, cos, atan2, sqrt, log, pow, exp, fmod) and compiles unchanged with Turbo C++ for DOS targets.

## Naming Conventions

- **`NAV_CONV_`** — unit conversion functions
- **`NAV_SUP_`** — all other navigation/support functions
- **`NAV_Sup_`** — struct type names (control filters, NMEA data)
- **`NAV_EXPORT`** — visibility attribute macro

## Directory Layout

```
NAV1/
├── dll.h              Master header (includes all modules)
├── export.h           Portability macro
├── dllmain.c          DllMain + core geodesy (8 functions)
├── conv.h / conv.c    Unit conversions (20 functions)
├── nav.h / nav.c      Advanced navigation (6 functions)
├── aviation.h / .c    Aviation math (5 functions)
├── gps.h / gps.c      GPS helpers (6 functions)
├── flight_mgmt.h / .c Flight planning (6 functions)
├── control.h / .c     Signal processing (5 structs, 10 functions)
├── nmea.h / nmea.c    NMEA parser (1 struct, 1 function)
├── test_nav1.c        117-test harness
├── test.bat            One-command compile + run
├── NAV1.dev            Dev-C++ project file
├── Makefile.win        Build rules
├── NAV1.ico            Application icon
├── NAV1_private.rc / .res  Windows resources
└── nav.bat             rundll32 launcher
```
