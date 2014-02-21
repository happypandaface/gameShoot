gcc -c -o build/gameShootSounds.o src/gameShootSounds.c -I./src/include
gcc -c -o build/gameShoot.o src/gameShoot.c -D FREEGLUT_STATIC -I./src/include
gcc -o build/gameShoot.exe build/*.o -L./lib -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -lOpenAL32 -lalut -Wl,--subsystem,windows
pause
call buildShoot.bat