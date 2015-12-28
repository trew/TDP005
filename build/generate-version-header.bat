@echo off

git describe --tags --long > temp.txt
set /p version=<temp.txt
del temp.txt

for /f "tokens=1,2,3 delims=." %%a in ("%version%") do set major=%%a&set minor=%%b&set rest=%%c

for /f "tokens=1,2,3 delims=-" %%a in ("%rest%") do set patch=%%a&set build=%%b&set commit=%%c

(
echo #define VER_FILE_VERSION %major%, %minor%, %patch%, %build%
echo #define VER_FILE_VERSION_STR "%major%.%minor%.%patch%.%build%"
echo #define VER_PRODUCT_VERSION %major%, %minor%, %patch%
echo #define VER_PRODUCT_VERSION_STR "%major%.%minor%.%patch%"
echo const char* const FULL_VERSION = "%major%.%minor%.%patch%.%build%-%commit%";
echo const char* const VERSION = "%major%.%minor%.%patch%";
) > ../include/version.h
