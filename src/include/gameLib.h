typedef enum { false, true } bool;

typedef enum {start, game, death} GameState;

typedef struct
{
	float x;
	float y;
} Pos2;

typedef struct
{
	float x;
	float y;
} Dim2;

struct Guy
{
	bool spawned;
	Pos2 pos;
	float spawnTime;
};

struct Bullet
{
	bool spawned;
	Pos2 pos;
	float traveled;
	int direction;
};