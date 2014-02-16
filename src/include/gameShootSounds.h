#ifndef GAME_SHOOT_SOUNDS_H
#define GAME_SHOOT_SOUNDS_H
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#define NUM_SOUNDS 4

#define SHOOT_SOUND 0
#define ZDEATH_SOUND 1
#define ZSPAWN_SOUND 2
#define PLAYER_DEATH_SOUND 3

ALuint shootSound;
ALuint zDeathSound;
ALuint zSpawnSound;
ALuint pDeathSound;

void playSound(int snd);
#endif