@echo off
if "%1"=="" (
    rundll32 NAV1.dll,Rundll32Version
) else (
    rundll32 NAV1.dll,RunTest %1
)
