@echo off

if "%1" == "" goto usage

set version=%1
set var=
set branch=

for /f %%i in ('git status --porcelain') do set var=%%i

if not "%var%" == "" (
  echo You have uncommitted changes.
  goto fail
)

git diff-index --quiet --cached HEAD
if not errorlevel 0 (
  echo You have uncommitted changes.
  goto fail
)

for /f %%i in ('git symbolic-ref --short HEAD') do set branch=%%i

if not "%branch%" == "develop" (
  echo You must be in the develop branch to run this command.
  goto fail
)

git checkout master

git merge develop --no-ff

git tag %version%

git checkout develop

git merge master

echo Release successfully merged and tagged as %version%!
echo Next steps include building it with Visual Studio and then creating a setup using Inno Setup.

:usage
echo Usage:
echo   release.bat ^<version to release^>

:fail