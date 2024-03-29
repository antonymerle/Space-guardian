#pragma once
typedef struct Entity Entity;
typedef struct Explosion Explosion;
typedef struct Debris Debris;
typedef struct Texture Texture;
typedef enum { NORMAL, MEGASHOT } ShotMode;

struct Texture {
	char name[MAX_NAME_LENGTH];
	SDL_Texture* texture;
	Texture* next;
};

typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} Subsystem;

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	Subsystem subsystem;
	Texture textureHead, *textureTail;
	int keyboard[MAX_KEYBOARD_KEYS];
	char inputText[MAX_LINE_LENGTH];
} App;

struct Entity {
	int x;
	int y;
	int w;
	int h;
	float dx;
	float dy;
	int side;
	int health;
	int reload;
	ShotMode shotMode;
	SDL_Texture* texture;
	SDL_Texture* trailer;
	Entity* next;
};

struct Explosion {
	float x;
	float y;
	float dx;
	float dy;
	int r, g, b, a;
	Explosion* next;
};

struct Debris {
	float x;
	float y;
	float dx;
	float dy;
	SDL_Rect rect;
	SDL_Texture* texture;
	int life;
	Debris* next;
};

typedef struct {
	Entity fighterHead;
	Entity* fighterTail;
	Entity bulletHead;
	Entity* bulletTail;
	Entity pointHead;
	Entity* pointTail;
	Explosion explosionHead;
	Explosion* explosionTail;
	Debris debrisHead;
	Debris* debrisTail;
	int score;
} Stage;

typedef struct {
	int x;
	int y;
	int speed;
} Star;

typedef struct {
	int recent;
	int score;
	char name[MAX_SCORE_NAME_LENGTH];
} Highscore;

typedef struct {
	int currentMinHighscore;
	Highscore highscore[NUM_HIGHSCORES];
} Highscores;