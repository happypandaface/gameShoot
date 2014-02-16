#include <gameShootSounds.h>

void playSound(int snd)
{
	switch (snd){
		case SHOOT_SOUND:
			alSourcePlay(shootSound);
			break;
		case ZDEATH_SOUND:
			alSourcePlay(zDeathSound);
			break;
		case ZSPAWN_SOUND:
			alSourcePlay(zSpawnSound);
			break;
		case PLAYER_DEATH_SOUND:
			alSourcePlay(pDeathSound);
			break;
	}
}
