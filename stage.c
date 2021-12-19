#include "stage.h"


static void logic(void);
static void draw(void);
static void drawPlayer(void);
static void drawBullets(void);
static void initPlayer(void);
static void doPlayer(void);
static void doBullets(void);
static void fireBullet(void);


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
	if(player) memset(player, 0, sizeof(Entity));

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

static void doPlayer()
{
	player->dx = 0;
	player->dy = 0;

	if (player->reload > 0) player->reload--;
	if (app.keyboard[SDL_SCANCODE_UP]) player->dy = -PLAYER_SPEED;
	if (app.keyboard[SDL_SCANCODE_DOWN]) player->dy = PLAYER_SPEED;
	if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
	if (app.keyboard[SDL_SCANCODE_RIGHT]) player->dx = PLAYER_SPEED;
	if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0) fireBullet();
	
	player->x += player->dx;
	player->y += player->dy;
}

static void fireBullet(void)
{
	Entity* bulletL;
	Entity* bulletR;

	bulletL = malloc(sizeof(Entity));
	bulletR = malloc(sizeof(Entity));
	if(bulletL) memset(bulletL, 0, sizeof(Entity));
	if(bulletR) memset(bulletR, 0, sizeof(Entity));

	stage.bulletTail->next = bulletL;
	stage.bulletTail = bulletL;

	stage.bulletTail->next = bulletR;
	stage.bulletTail = bulletR;

	bulletL->x = player->x + player->w / 2;
	bulletL->y = player->y;
	bulletL->dx = PLAYER_BULLET_SPEED;
	bulletL->dy = 0;
	bulletL->health = 1;
	bulletL->texture = bulletTexture;
	SDL_QueryTexture(bulletL->texture, NULL, NULL, &bulletL->w, &bulletL->h);

	bulletR->x = player->x + player->w / 2;
	bulletR->y = player->y + player->h;
	bulletR->dx = PLAYER_BULLET_SPEED;
	bulletR->dy = 0;
	bulletR->health = 1;
	bulletR->texture = bulletTexture;
	bulletR->w = bulletL->w;
	bulletR->h = bulletL->h;

	/* 8 frames (approx 0.133333 seconds) must pass before we can fire again. */
	player->reload = 4;
}

static void doBullets()
{
	Entity* b;
	Entity* prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (b->x > SCREEN_WIDTH)
		{
			if (b == stage.bulletTail) stage.bulletTail = prev;

			prev->next = b->next;
			free(b);
			b = prev;
		}
		prev = b;
	}
}

static void draw(void)
{
	drawPlayer();
	drawBullets();
}

static void drawPlayer(void)
{
	blit(player->texture, player->x, player->y);
}

static void drawBullets(void)
{
	Entity* b;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		blit(b->texture, b->x, b->y);
	}
}

