@echo off
setlocal

set CC="C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc.exe"
set PATH=C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin;%PATH%

echo Compiling test_nav1.exe ...
%CC% test_nav1.c -o test_nav1.exe -Isrc -L. -lNAV1
if %ERRORLEVEL% neq 0 (
    echo Compilation failed!
    exit /b 1
)

echo.
echo ================================
echo  Running Test Suite
echo ================================
echo.
test_nav1.exe
set RESULT=%ERRORLEVEL%
echo.
if %RESULT% equ 0 (
    echo All tests passed.
) else (
    echo Some tests FAILED.
)
exit /b %RESULT%
