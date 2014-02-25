all:
	cc -c -o build/loadPNGtoGL.o src/loadPNGtoGL.c -I./src/include
	cc -c -o build/gameShootSounds.o src/gameShootSounds.c -I./src/include
	cc -c -o build/gameShoot.o src/gameShoot.c -DGLEW_STATIC -I./src/include
	cc -o build/gameShoot build/*.o -lpng -lGL -lGLEW -lGLU -lglut -lalut

glew:
	gcc -c -o build/glew.o src/glew.c -DGLEW_STATIC -I./src/include
