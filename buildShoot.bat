gcc -c -o build/gameShoot.o src/gameShoot.c -D FREEGLUT_STATIC -I./include
gcc -o build/gameShoot.exe build/gameShoot.o -L./lib -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -lOpenAL32 -lalut -Wl,--subsystem,windows
pause
call buildShoot.bat