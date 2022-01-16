#pragma once
typedef struct Entity Entity;
typedef struct Explosion Explosion;
typedef struct Debris Debris;
typedef enum { normal, powerful } ShotMode;

typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} Delegate;

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS];
} App;

//typedef struct {
//	SDL_Texture* trailerTexture;
//	int a;
//} Trailer;

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