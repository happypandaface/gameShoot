g++ -c -o build/loadPNGtoGL.o src/loadPNGtoGL.c -I./src/include
gcc -c -o build/gameShootSounds.o src/gameShootSounds.c -I./src/include
gcc -c -o build/gameShoot.o src/gameShoot.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./src/include
g++ -o build/gameShoot.exe build/*.o -L./lib -lpng -lzlibstatic -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -lOpenAL32 -lalut -Wl,--subsystem,windows
pause
call buildShoot.bat