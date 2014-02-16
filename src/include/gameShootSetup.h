
int gameTime = 0;
float triPos = 0.5;
bool up,down,left,right;
bool wDown,aDown,sDown,dDown;
float cooldown = 0;
float spawnCooldown;
float survivalTime = 0;

GameState gameState;

struct Guy guy;

struct Guy enemies[MAX_ENEMIES];
struct Bullet bullets[MAX_BULLETS];
