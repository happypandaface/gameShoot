gcc -c -o gameShoot.o gameShoot.c -D FREEGLUT_STATIC -I./include
gcc -o gameShoot.exe gameShoot.o -L./lib -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -lOpenAL32 -lalut -Wl,--subsystem,windows
pause
call buildShoot.bat