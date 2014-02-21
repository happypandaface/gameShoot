#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

#include <gameLib.h>
#include <gameShootConsts.h>
#include <gameShootSetup.h>
#include <gameShootSounds.h>

ShootGame shootGame;

void resetGame();
void startGame();

void keyboardDown(unsigned char key, int x, int y)
{
	switch(key){
		case 'w':
			shootGame.input.wDown = true;
			break;
		case 'a':
			shootGame.input.aDown = true;
			break;
		case 's':
			shootGame.input.sDown = true;
			break;
		case 'd':
			shootGame.input.dDown = true;
			break;
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == ' ')
	{
		if (shootGame.gameState == start)
		{
			startGame();
			shootGame.gameState = game;
		}else if (shootGame.gameState == death)
		{
			resetGame();
			shootGame.gameState = start;
		}
	}
	switch(key){
		case 'w':
			shootGame.input.wDown = false;
			break;
		case 'a':
			shootGame.input.aDown = false;
			break;
		case 's':
			shootGame.input.sDown = false;
			break;
		case 'd':
			shootGame.input.dDown = false;
			break;
	}
}

void keyboardSpecialDown(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_UP :
			shootGame.input.up = true;
		break;
		case GLUT_KEY_DOWN :
			shootGame.input.down = true;
		break;
		case GLUT_KEY_LEFT :
			shootGame.input.left = true;
		break;
		case GLUT_KEY_RIGHT :
			shootGame.input.right = true;
		break;
		default:
			break;
	}
}

void keyboardSpecialUp(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_UP :
			shootGame.input.up = false;
		break;
		case GLUT_KEY_DOWN :
			shootGame.input.down = false;
		break;
		case GLUT_KEY_LEFT :
			shootGame.input.left = false;
		break;
		case GLUT_KEY_RIGHT :
			shootGame.input.right = false;
		break;
		default:
			break;
	}
}

void resetGame()
{
	int i = 0;
	while (i < MAX_ENEMIES)
	{
		enemies[i].spawned = false;
		++i;
	}
	i = 0;
	while (i < MAX_BULLETS)
	{
		bullets[i].spawned = false;
		++i;
	}
	guy.pos.x = 0;
	guy.pos.y = 0;
}

void startGame()
{
	shootGame.survivalTime = 0;
}

bool checkCollision(Pos2 obj1, Dim2 dim1, Pos2 obj2, Dim2 dim2)
{
	return (obj1.x + dim1.x > obj2.x - dim2.x &&
			obj1.x - dim1.x < obj2.x + dim2.x &&
			obj1.y + dim1.y > obj2.y - dim2.y &&
			obj1.y - dim1.y < obj2.y + dim2.y);
}

float angle = 0.0f;
float lastTime = 0;

void fireBullet(int direction)
{
	playSound(SHOOT_SOUND);
	shootGame.cooldown = GUN_COOLDOWN;
	int i = 0;
	while(i < MAX_BULLETS)
	{
		if (bullets[i].spawned == false)
		{
			bullets[i].spawned = true;
			bullets[i].pos.x = guy.pos.x;
			bullets[i].pos.y = guy.pos.y;
			bullets[i].direction = direction;
			bullets[i].traveled = 0;
			break;
		}
		++i;
	}
}

void spawnEnemy(float x, float y)
{
	playSound(ZSPAWN_SOUND);
	shootGame.spawnCooldown = SPAWN_COOLDOWN-sqrt(SPAWN_COOLDOWN_MOD*shootGame.survivalTime);
	int i = 0;
	while(i < MAX_ENEMIES)
	{
		if (enemies[i].spawned == false)
		{
			enemies[i].spawned = true;
			enemies[i].pos.x = x;
			enemies[i].pos.y = y;
			enemies[i].spawnTime = 0;
			break;
		}
		++i;
	}
}

float getRand()
{
	return (float)((rand() % 100) + 1)/100.0f;
}

void renderScene(void)
{
	float gameTime=glutGet(GLUT_ELAPSED_TIME);
	float delta;
	if (lastTime == 0)
		delta = 0;
	else
		delta = (gameTime-lastTime)/1000;
	lastTime = gameTime;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (shootGame.gameState == game || shootGame.gameState == death)
	{
		if (shootGame.gameState == death)
			delta = 0;
		shootGame.survivalTime += delta;
		// Reset transformations
		glLoadIdentity();
		// Set the camera
		
		//gluLookAt(	0.0f, 0.0f, 10.0f,
		//		0.0f, 0.0f,  0.0f,
		//		0.0f, 1.0f,  0.0f);
				
		
		//glRotatef(angle, 0.0f, 1.0f, 0.0f);
		//glColor3f(red,green,blue);
		
		if (shootGame.spawnCooldown > 0)
			shootGame.spawnCooldown -= delta;
		else
		{
			spawnEnemy(
				getRand()*2.0f-1.0f, 
				getRand()*2.0f-1.0f);
			//spawnEnemy(-0.5f, -0.5f);
		}
		
		if (shootGame.cooldown > 0)
			shootGame.cooldown -= delta;
		else
		{
			if (shootGame.input.wDown)
				fireBullet(UP);
			else if (shootGame.input.aDown)
				fireBullet(LEFT);
			else if (shootGame.input.sDown)
				fireBullet(DOWN);
			else if (shootGame.input.dDown)
				fireBullet(RIGHT);
		}
		
		if (shootGame.input.up == true)
			guy.pos.y += delta*MOVE_SPEED;
		if (shootGame.input.down == true)
			guy.pos.y -= delta*MOVE_SPEED;
		if (shootGame.input.right == true)
			guy.pos.x += delta*MOVE_SPEED;
		if (shootGame.input.left == true)
			guy.pos.x -= delta*MOVE_SPEED;
		if (guy.pos.x > 1)
			guy.pos.x = 1;
		if (guy.pos.x < -1)
			guy.pos.x = -1;
		if (guy.pos.y > 1)
			guy.pos.y = 1;
		if (guy.pos.y < -1)
			guy.pos.y = -1;
		
		glColor3f(1.0f,0.0f,0.0f);
		glBegin(GL_QUADS);
			glVertex3f(guy.pos.x-GUY_SIZE,guy.pos.y-GUY_SIZE,0.0);
			glVertex3f(guy.pos.x+GUY_SIZE,guy.pos.y-GUY_SIZE,0.0);
			glVertex3f(guy.pos.x+GUY_SIZE,guy.pos.y+GUY_SIZE,0.0);
			glVertex3f(guy.pos.x-GUY_SIZE,guy.pos.y+GUY_SIZE,0.0);
		glEnd();
		
		int i = 0;
		while (i < MAX_ENEMIES)
		{
			if (enemies[i].spawned == true)
			{
				bool drawEnemy = true;
				if (enemies[i].spawnTime > SPAWN_TIME)
				{
					if (shootGame.gameState == game &&
						checkCollision(enemies[i].pos, ENEMY_DIM, guy.pos, PLAYER_DIM))
					{
						playSound(PLAYER_DEATH_SOUND);
						shootGame.gameState = death;
					}
					if (guy.pos.x < enemies[i].pos.x-MOVE_SPEED_ENEMY_1*delta)
						enemies[i].pos.x -= MOVE_SPEED_ENEMY_1*delta;
					if (guy.pos.y < enemies[i].pos.y-MOVE_SPEED_ENEMY_1*delta)
						enemies[i].pos.y -= MOVE_SPEED_ENEMY_1*delta;
					if (guy.pos.x > enemies[i].pos.x+MOVE_SPEED_ENEMY_1*delta)
						enemies[i].pos.x += MOVE_SPEED_ENEMY_1*delta;
					if (guy.pos.y > enemies[i].pos.y+MOVE_SPEED_ENEMY_1*delta)
						enemies[i].pos.y += MOVE_SPEED_ENEMY_1*delta;
				}else
				{
					enemies[i].spawnTime += delta;
					if (shootGame.gameState != death && ((int)(enemies[i].spawnTime*BLINK_RATE))%2 == 0)
						drawEnemy = false;
				}
				if (drawEnemy == true)
				{
					glColor3f(0.0f,1.0f,0.0f);
					glBegin(GL_QUADS);
						glVertex3f(enemies[i].pos.x-ENEMY_SIZE,enemies[i].pos.y-ENEMY_SIZE,0.0);
						glVertex3f(enemies[i].pos.x+ENEMY_SIZE,enemies[i].pos.y-ENEMY_SIZE,0.0);
						glVertex3f(enemies[i].pos.x+ENEMY_SIZE,enemies[i].pos.y+ENEMY_SIZE,0.0);
						glVertex3f(enemies[i].pos.x-ENEMY_SIZE,enemies[i].pos.y+ENEMY_SIZE,0.0);
					glEnd();
				}
			}
			++i;
		}

		i = 0;
		while (i < MAX_BULLETS)
		{
			if (bullets[i].spawned == true)
			{
				int c = 0;
				while (c < MAX_ENEMIES)
				{
					if (enemies[c].spawned == true && enemies[c].spawnTime > SPAWN_TIME)
					{
						if (checkCollision(enemies[c].pos, ENEMY_DIM, bullets[i].pos, BULLET_DIM))
						{
							playSound(ZDEATH_SOUND);
							enemies[c].spawned = false;
							bullets[i].spawned = false;
							break;
						}
					}
					++c;
				}
				bullets[i].traveled += delta;
				if (bullets[i].traveled > BULLET_DISTANCE)
					bullets[i].spawned = false;
				if (bullets[i].spawned)// might've gotten destroyed
				{
					if (bullets[i].direction == LEFT)
						bullets[i].pos.x -= BULLET_SPEED*delta;
					if (bullets[i].direction == DOWN)
						bullets[i].pos.y -= BULLET_SPEED*delta;
					if (bullets[i].direction == RIGHT)
						bullets[i].pos.x += BULLET_SPEED*delta;
					if (bullets[i].direction == UP)
						bullets[i].pos.y += BULLET_SPEED*delta;
					glColor3f(0.0f,1.0f,1.0f);
					glBegin(GL_QUADS);
						glVertex3f(bullets[i].pos.x-BULLET_SIZE,bullets[i].pos.y-BULLET_SIZE,0.0);
						glVertex3f(bullets[i].pos.x+BULLET_SIZE,bullets[i].pos.y-BULLET_SIZE,0.0);
						glVertex3f(bullets[i].pos.x+BULLET_SIZE,bullets[i].pos.y+BULLET_SIZE,0.0);
						glVertex3f(bullets[i].pos.x-BULLET_SIZE,bullets[i].pos.y+BULLET_SIZE,0.0);
					glEnd();
				}
			}
			++i;
		}
	}
	else if (shootGame.gameState == start)
	{
		//glColor3f(1.0f,1.0f,1.0f);
		glRasterPos2f(-0.3f, 0.2f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "THIS GAME");
		glRasterPos2f(-0.6f, 0.1f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "Use the arrow keys to move");
		glRasterPos2f(-0.58f, 0.0f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "Use w, a, s and d to fire");
		glRasterPos2f(-0.5f, -0.1f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "Press space to start");
	}
	if (shootGame.gameState == death)
	{
		//glColor3f(1.0f,1.0f,1.0f);
		glRasterPos2f(-0.54f, 0.2f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "YOU'VE BEEN KILLED!");
		glRasterPos2f(-0.67f, 0.1f);
		char timeStr[256];
		sprintf(timeStr, "You survived for: %d seconds", ((int)shootGame.survivalTime));
		glutBitmapString(GLUT_BITMAP_8_BY_13, timeStr);
		glRasterPos2f(-0.56f, 0.0f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "Press space to continue");
	}
	
	glutSwapBuffers();
}

void changeSize(int w, int h)
{
	if (w != WINDOW_WIDTH && h != WINDOW_HEIGHT)
		glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void audioCleanUp()
{
    // Clean up sources and buffers
    //alDeleteSources(1, &source);
    //alDeleteBuffers(1, &buffer);

    // Exit everything
    alutExit();
}

#define FILENAME "sample.wav"


const GLchar* vertexShaderSource = "\n\
#version 150\n\
in vec3 vertex;\n\
varying vec3 position;\n\
void\n\
main()\n\
{\n\
	position = vertex;\n\
    gl_Position = vec4(vertex,1.0);\n\
}\n\
";

const GLchar* fragmentShaderSource = "\n\
#version 150\n\
out vec4 fragmentColor;\n\
varying vec3 position;\n\
void\n\
main()\n\
{\n\
    float x_dis = position.x;\n\
    float y_dis = position.y;\n\
	float dst = sqrt(pow(x_dis, 2.0) + pow(y_dis, 2.0));\n\
	if (dst < 0.5)\n\
		fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);\n\
	else\n\
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);\n\
}\n\
";

int main(int argc, char **argv)
{
	
	printf("Elapsed seconds\n");
	fflush(stdout);
	
	
    alutInit(0, NULL);
    alGetError();
	ALuint state;
	
	int i = 0;
	while (i < NUM_SOUNDS)
	{
		ALuint buffer;
		switch (i){
			case SHOOT_SOUND:
				buffer = alutCreateBufferFromFile("Laser_Shoot20.wav");
				alGenSources(1, &shootSound);
				alSourcei(shootSound, AL_BUFFER, buffer);
				break;
			case ZDEATH_SOUND:
				buffer = alutCreateBufferFromFile("Explosion45.wav");
				alGenSources(1, &zDeathSound);
				alSourcei(zDeathSound, AL_BUFFER, buffer);
				break;
			case ZSPAWN_SOUND:
				buffer = alutCreateBufferFromFile("Powerup19.wav");
				alGenSources(1, &zSpawnSound);
				alSourcei(zSpawnSound, AL_BUFFER, buffer);
				break;
			case PLAYER_DEATH_SOUND:
				buffer = alutCreateBufferFromFile("file3.raw");
				alGenSources(1, &pDeathSound);
				alSourcei(pDeathSound, AL_BUFFER, buffer);
				break;
		}
		++i;
	}
	
    // Play
	//alSourcePlay(source);	
	
	srand(time(0));
	guy.pos.x = 0;
	guy.pos.y = 0;
	shootGame.spawnCooldown = SPAWN_COOLDOWN;
	
	// init GLUT and create Window
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Kill the zombies");
	
	//glutKeyboardFunc(keyboard);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);
	
	GLenum err = glewInit();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glUseProgram(program);
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// enter GLUT event processing cycle
	glutMainLoop();
	
	return 1;

}