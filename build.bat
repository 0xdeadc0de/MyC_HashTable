cd MyC_HashTable
py "../MyC.py"
cd ..
rd /s /q build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make.exe