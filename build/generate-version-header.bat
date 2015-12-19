@echo off

git describe --tags --long > temp.txt
set /p version=<temp.txt
del temp.txt

for /f "tokens=1,2 delims=." %%a in ("%version%") do set major=%%a&set rest=%%b

for /f "tokens=1,2,3 delims=-" %%a in ("%rest%") do set minor=%%a&set patch=%%b&set build=%%c

(
echo #define VER_FILE_VERSION %major%, %minor%, %patch%
echo #define VER_FILE_VERSION_STR "%major%.%minor%.%patch%"
echo #define VER_PRODUCT_VERSION VER_FILE_VERSION
echo #define VER_PRODUCT_VERSION_STR VER_FILE_VERSION_STR
echo const char* const FULL_VERSION = "%major%.%minor%.%patch%-%build%";
echo const char* const VERSION = "%major%.%minor%.%patch%";
) > ../include/version.h
