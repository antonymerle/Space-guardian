#include "stage.h"


static void logic(void);
static void draw(void);
static void initPlayer(void);
static void doPlayer(void);
static void doBullets(void);


static Entity* player = NULL;
static SDL_Texture* bulletTexture = NULL;


void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(&stage, 0, sizeof(Stage));

	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;

	initPlayer();

	bulletTexture = loadTexture("gfx/playerBullet.png");
}

static void initPlayer(void)
{
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));

	stage.fighterTail->next = player;
	stage.fighterTail = player;

	player->x = 100;
	player->y = 100;
	player->texture = loadTexture("gfx/player.png");

	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void)
{
	doPlayer();
	doBullets();
}

static void draw(void) {}

static void doPlayer() {}
static void doBullet() {}

