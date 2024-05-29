@echo off
echo.
cd MyC_HashTable
py "../MyC.py"

if %errorlevel% neq 0 (
  echo[
  echo An error occurred during the previous command. Exiting script.
  exit /b 1
)

cd ..
rd /s /q build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..

if %errorlevel% neq 0 (
  echo(
  echo An error occurred during the previous command. Exiting script.
  exit /b 1
)

mingw32-make.exe