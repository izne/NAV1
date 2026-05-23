# NAV1 -- a collection of navigation-related helper functions in C89

A static and shared library providing geodesy, unit conversion, aviation, GPS/NMEA, flight management, and control-system math for autopilot and navigation applications.

## Quick Start

```batch
test.bat          -- compile test harness + run all tests
```

Output: `=== Results: 289/289 passed ===`

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
| Core / Geodesy | `NAV1_GEO_` | `geo.c` / `geo.h`     | 10          | Distance, bearing, destination, midpoint, cross-track, DMS, ECEF |
| DLL Entry      | --           | `nav1.c` / `nav1.h`   | 2           | Version, WinVer |
| Conversions    | `NAV1_CONV_` | `conv.c` / `conv.h`   | 20          | Length, speed, temperature, pressure |
| Navigation     | `NAV1_NAV_` | `nav.c` / `nav.h`     | 7           | Along-track, waypoints, rhumb line, antipode, compass, closest point on course |
| Aviation       | `NAV1_AERO_` | `aero.c` / `aero.h`   | 8           | Wind correction, ISA atmosphere, pressure/density altitude, speed of sound, Mach, CAS→TAS |
| GPS            | `NAV1_GPS_` | `gps.c` / `gps.h`     | 6           | DMM conversion, GPS speed, NMEA checksum, lat/lon validation |
| Flight Mgmt    | `NAV1_FLT_` | `flight_mgmt.c` / `flight_mgmt.h` | 6 | Time to altitude, VS, TOD, waypoint, fuel |
| Control        | `NAV1_CTL_` | `control.c` / `control.h` | 10 (+ 5 structs) | LPF, slew limiter, moving avg, PID, complementary filter |
| NMEA           | `NAV1_NMEA_` | `nmea.c` / `nmea.h` | 1 (+ 1 struct) | Parse $--GGA / $--RMC sentences |
| Route          | `NAV1_RTE_` | `route.c` / `route.h` | 11 (+ 2 structs) | Waypoint list, sequencing, CSV I/O |
| X-Plane Nav    | `NAV1_XPL_` | `xplane_nav.c` / `xplane_nav.h` | 6 (+ 2 structs) | Nav database loader, ICAO FPL parser |
| ARINC 429      | `NAV1_A429_` | `arinc429.c` / `arinc429.h` | 13 (+ 1 struct, 41 labels) | Word encoder/decoder, BNR/BCD, parity, label lookup |
| ARINC 424      | `NAV1_A424_` | `arinc424.c` / `arinc424.h` | 2 (+ 6 structs) | Nav database parser (airports, waypoints, runways, ILS) |
| METAR          | `NAV1_METAR_` | `metar.c` / `metar.h` | 4 (+ 3 structs) | METAR parser: parse, wind avg, crosswind, headwind |

## NMEA Parser

Parses NMEA 0183 `$--GGA` and `$--RMC` sentences from any talker (GP, GN, GL, GA).

```c
NAV1_NMEAData nd;
if (NAV1_NMEA_nmeaParse("$GPGGA,...*47", &nd)) {
    printf("lat=%.5f lon=%.5f alt=%.1f\n",
           nd.latitude, nd.longitude, nd.altitudeM);
}
```

## Test System

- `test_nav1.c` -- standalone harness, links against `libNAV1.a`
- Macros: `TEST_NEAR`, `TEST_TRUE`, `TEST_STR`
- Exit code 0 = all pass, 1 = any failure
- Coverage: every exported function, edge cases included

## Portability

`export.h` provides the `NAV1_EXPORT` macro:

| Build context          | `NAV1_EXPORT` resolves to    |
|------------------------|------------------------------|
| Windows DLL build      | `__declspec(dllexport)`      |
| Windows consumer       | `__declspec(dllimport)`      |
| Borland C++ DLL build  | `__export`                   |
| Static library / DOS   | *(empty -- no decoration)*   |

The math itself is pure C89 (sin, cos, atan2, sqrt, log, pow, exp, fmod).

## Naming Conventions

- **`NAV1_`** prefix on every function -- `NAV1_GEO_`, `NAV1_CONV_`, `NAV1_NAV_`, `NAV1_AERO_`, `NAV1_GPS_`, `NAV1_FLT_`, `NAV1_CTL_`, `NAV1_NMEA_`, `NAV1_RTE_`, `NAV1_XPL_`, `NAV1_A429_`, `NAV1_A424_`, `NAV1_METAR_`
- **`NAV1_`** -- struct type names (e.g. `NAV1_Waypoint`, `NAV1_Route`, `NAV1_NMEAData`)
- **`NAV1_`** prefix on all macros/constants (e.g. `NAV1_A429_ENCODING_BNR`, `NAV1_METAR_MAX_CLOUDS`, `NAV1_NAV_VOR`)
- **`NAV1_EXPORT`** -- visibility attribute macro

## Directory Layout

```
NAV1/
|-- src/
|   |-- export.h           Portability macro (NAV1_EXPORT)
|   |-- nav1.h             Internal master header (includes all sub-headers)
|   |-- nav1.c             DllMain + Version, WinVer
|   |-- geo.h / geo.c      Core geodesy (10 functions)
|   |-- conv.h / conv.c    Unit conversions (20 functions)
|   |-- nav.h / nav.c      Advanced navigation (7 functions)
|   |-- aero.h / aero.c    Aviation math (8 functions)
|   |-- gps.h / gps.c      GPS helpers (6 functions)
|   |-- flight_mgmt.h / .c Flight planning (6 functions)
|   |-- control.h / .c     Signal processing (5 structs, 10 functions)
|   |-- nmea.h / nmea.c    NMEA parser (1 struct, 1 function)
|   |-- route.h / route.c  Route management + CSV I/O (11 functions)
|   |-- xplane_nav.h / .c  X-Plane nav database + ICAO FPL (6 functions)
|   |-- arinc429.h / .c    ARINC 429 word encoder/decoder (13 functions, 41 labels)
|   |-- arinc424.h / .c    ARINC 424 nav database parser (2 functions, 6 structs)
|-- NAV1.h                 Public API header (single include)
|-- test_nav1.c            289-test harness
|-- test.bat               One-command compile + run
|-- NAV1.dev               Dev-C++ project file
|-- Makefile.win            Build rules
|-- NAV1.ico                Application icon
|-- NAV1_private.rc / .res  Windows resources
|-- nav.bat                 rundll32 launcher
```
