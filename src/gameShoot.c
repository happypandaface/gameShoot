#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

#include <gameLib.h>
#include <gameShootConsts.h>
#include <gameShootSetup.h>
#include <gameShootSounds.h>

void keyboardDown(unsigned char key, int x, int y)
{
	switch(key){
		case 'w':
			wDown = true;
			break;
		case 'a':
			aDown = true;
			break;
		case 's':
			sDown = true;
			break;
		case 'd':
			dDown = true;
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
	survivalTime = 0;
}

void keyboardUp(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == ' ')
	{
		if (gameState == start)
		{
			startGame();
			gameState = game;
		}else if (gameState == death)
		{
			resetGame();
			gameState = start;
		}
	}
	switch(key){
		case 'w':
			wDown = false;
			break;
		case 'a':
			aDown = false;
			break;
		case 's':
			sDown = false;
			break;
		case 'd':
			dDown = false;
			break;
	}
}

void keyboardSpecialDown(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_UP :
			up = true;
		break;
		case GLUT_KEY_DOWN :
			down = true;
		break;
		case GLUT_KEY_LEFT :
			left = true;
		break;
		case GLUT_KEY_RIGHT :
			right = true;
		break;
		default:
			break;
	}
}

void keyboardSpecialUp(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_UP :
			up = false;
		break;
		case GLUT_KEY_DOWN :
			down = false;
		break;
		case GLUT_KEY_LEFT :
			left = false;
		break;
		case GLUT_KEY_RIGHT :
			right = false;
		break;
		default:
			break;
	}
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
	cooldown = GUN_COOLDOWN;
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
	spawnCooldown = SPAWN_COOLDOWN-sqrt(SPAWN_COOLDOWN_MOD*survivalTime);
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
	
	if (gameState == game || gameState == death)
	{
		if (gameState == death)
			delta = 0;
		survivalTime += delta;
		// Reset transformations
		glLoadIdentity();
		// Set the camera
		/*
		gluLookAt(	0.0f, 0.0f, 10.0f,
				0.0f, 0.0f,  0.0f,
				0.0f, 1.0f,  0.0f);
				*/
		
		//glRotatef(angle, 0.0f, 1.0f, 0.0f);
		//glColor3f(red,green,blue);
		
		if (spawnCooldown > 0)
			spawnCooldown -= delta;
		else
		{
			spawnEnemy(
				getRand()*2.0f-1.0f, 
				getRand()*2.0f-1.0f);
			//spawnEnemy(-0.5f, -0.5f);
		}
		
		if (cooldown > 0)
			cooldown -= delta;
		else
		{
			if (wDown)
				fireBullet(UP);
			else if (aDown)
				fireBullet(LEFT);
			else if (sDown)
				fireBullet(DOWN);
			else if (dDown)
				fireBullet(RIGHT);
		}
		
		if (up == true)
			guy.pos.y += delta*MOVE_SPEED;
		if (down == true)
			guy.pos.y -= delta*MOVE_SPEED;
		if (right == true)
			guy.pos.x += delta*MOVE_SPEED;
		if (left == true)
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
					if (gameState == game &&
						checkCollision(enemies[i].pos, ENEMY_DIM, guy.pos, PLAYER_DIM))
					{
						playSound(PLAYER_DEATH_SOUND);
						gameState = death;
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
					if (gameState != death && ((int)(enemies[i].spawnTime*BLINK_RATE))%2 == 0)
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
						if (enemies[c].pos.x + ENEMY_SIZE > bullets[i].pos.x - BULLET_SIZE &&
							enemies[c].pos.x - ENEMY_SIZE < bullets[i].pos.x + BULLET_SIZE &&
							enemies[c].pos.y + ENEMY_SIZE > bullets[i].pos.y - BULLET_SIZE &&
							enemies[c].pos.y - ENEMY_SIZE < bullets[i].pos.y + BULLET_SIZE)
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
	else if (gameState == start)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glRasterPos2f(-0.3f, 0.2f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "THIS GAME");
		glRasterPos2f(-0.6f, 0.1f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "Use the arrow keys to move");
		glRasterPos2f(-0.58f, 0.0f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "Use w, a, s and d to fire");
		glRasterPos2f(-0.5f, -0.1f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "Press space to start");
	}
	if (gameState == death)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glRasterPos2f(-0.54f, 0.2f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, "YOU'VE BEEN KILLED!");
		glRasterPos2f(-0.67f, 0.1f);
		char timeStr[256];
		sprintf(timeStr, "You survived for: %d seconds", ((int)survivalTime));
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

int main(int argc, char **argv)
{
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
	spawnCooldown = SPAWN_COOLDOWN;
	
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
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// enter GLUT event processing cycle
	glutMainLoop();
	
	return 1;

}