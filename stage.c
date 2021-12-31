#include "stage.h"

static void logic(void);
static void draw(void);
static void drawPlayer(void);
static void drawBullets(void);
static void initPlayer(void);
static void doPlayer(void);
static void doBullets(void);
static void fireBullet(void);
static int bulletHitFighter(Entity* b);
static int generateRandomNumber(unsigned int top);
static void doFighters(void);
static void spawnEnemies(void);
static void drawFighters(void);
static void resetStage(void);
static void doEnemies(void);
static void fireAlienBullet(Entity* e);
static void cadrePlayer(void);


static Entity* player;
static SDL_Texture* playerTexture;
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;
static SDL_Texture* alienBulletTexture;
static SDL_Texture* megaShot;
static int enemySpawnTimer;
static int stageResetTimer;

/* DEBUG */
static unsigned int bulletNumber;
static unsigned int hitCount;

void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(&stage, 0, sizeof(Stage));

	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;

	initPlayer();

	playerTexture = loadTexture("gfx/player.png");
	bulletTexture = loadTexture("gfx/playerBullet.png");
	enemyTexture = loadTexture("gfx/enemy.png");
	alienBulletTexture = loadTexture("gfx/alienBullet.png");
	megaShot = loadTexture("gfx/MegaShot.png");

	//enemySpawnTimer = 0;
	bulletNumber = 0;
	hitCount = 0;

	resetStage();
}

static void resetStage(void)
{
	Entity* e;

	while (stage.fighterHead.next)
	{
		e = stage.fighterHead.next;
		stage.fighterHead.next = e->next;
		free(e);
	}

	while (stage.bulletHead.next)
	{
		e = stage.bulletHead.next;
		stage.bulletHead.next = e->next;
		free(e);
		bulletNumber--;
	}

	memset(&stage, 0, sizeof(Stage));
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;

	initPlayer();

	enemySpawnTimer = 0;
	stageResetTimer = FPS * 2;
}

static void initPlayer(void)
{
	player = malloc(sizeof(Entity));
	if (player) memset(player, 0, sizeof(Entity));

	stage.fighterTail->next = player;
	stage.fighterTail = player;

	player->health = 3;
	player->side = SIDE_PLAYER;
	player->x = 100;
	player->y = 100;

	player->texture = playerTexture;

	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void)
{
		doPlayer();
		doEnemies();
		doFighters();
		doBullets();
		spawnEnemies();
		cadrePlayer();
		/*printf("%u\n", bulletNumber);*/
		if (player == NULL && --stageResetTimer <= 0) resetStage();
}

static void doPlayer(void)
{
	if (player)
	{
		player->dx = 0;
		player->dy = 0;

		if (player->reload > 0) player->reload--;
		if (app.keyboard[SDL_SCANCODE_UP]) player->dy = -PLAYER_SPEED;
		if (app.keyboard[SDL_SCANCODE_DOWN]) player->dy = PLAYER_SPEED;
		if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
		if (app.keyboard[SDL_SCANCODE_RIGHT]) player->dx = PLAYER_SPEED;
		if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0) fireBullet();
	}
}

static void cadrePlayer(void)
{
	if (player)
	{
		if (player->x < 0) player->x = 0;
		if (player->y < 0) player->y = 0;
		if (player->x > SCREEN_WIDTH / 2) player->x = SCREEN_WIDTH / 2;
		if (player->y > SCREEN_HEIGHT - player->h) player->y = SCREEN_HEIGHT - player->h;
	}
}

static void fireBullet(void)
{
	Entity* bulletL;
	Entity* bulletR;

	bulletL = malloc(sizeof(Entity));
	bulletR = malloc(sizeof(Entity));
	if (bulletL) memset(bulletL, 0, sizeof(Entity));
	if (bulletR) memset(bulletR, 0, sizeof(Entity));

	stage.bulletTail->next = bulletL;
	stage.bulletTail = bulletL;

	stage.bulletTail->next = bulletR;
	stage.bulletTail = bulletR;

	bulletL->side = SIDE_PLAYER;
	bulletL->x = player->x + player->w / 2;
	bulletL->y = player->y;
	bulletL->dx = PLAYER_BULLET_SPEED;
	bulletL->dy = 0;
	bulletL->health = 1;
	bulletL->texture = bulletTexture;
	SDL_QueryTexture(bulletL->texture, NULL, NULL, &bulletL->w, &bulletL->h);
	bulletNumber++;

	bulletR->side = SIDE_PLAYER;
	bulletR->x = player->x + player->w / 2;
	bulletR->y = player->y + player->h;
	bulletR->dx = PLAYER_BULLET_SPEED;
	bulletR->dy = 0;
	bulletR->health = 1;
	bulletR->texture = bulletTexture;
	bulletR->w = bulletL->w;
	bulletR->h = bulletL->h;
	bulletNumber++;


	/* 8 frames (approx 0.133333 seconds) must pass before we can fire again. */
	player->reload = 8;
}

static void doBullets(void)
{
	Entity* b;
	Entity* prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (bulletHitFighter(b) || b->x > SCREEN_WIDTH || b->x <= 0 || b->y > SCREEN_HEIGHT || b->y <= 0/*-b->h*/ || (b->dx == 0) && (b->dy == 0))
		{
			if (b == stage.bulletTail) stage.bulletTail = prev;

			prev->next = b->next;
			free(b);
			b = prev;

			bulletNumber--;
		}
		prev = b;
	}
}

static int bulletHitFighter(Entity* b)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if(e->side != b->side 
			&& collision(e->x, e->y, e->w, e->h, b->x, b->y, b->w, b->h))
		{
			hitCount++;
			// printf("%u hit\n", hitCount);
			b->health = 0;
			e->health--;

			return 1;
		}
		//printf("miss\n");
	}
	//SDL_Log("hum\n");

	return 0;
}

static void draw(void)
{
	drawBullets();
	drawFighters();
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

static int generateRandomNumber(unsigned int top)
{
	/* Intializes random number generator */
	unsigned int seed;
	char randomNegativeSwitch;

	seed = SDL_GetTicks();
	randomNegativeSwitch = seed % 2 ? 1 : -1;

	return (rand() % top) * randomNegativeSwitch;
}

static void doFighters(void)
{
	Entity* e;
	Entity* prev;

	prev = &stage.fighterHead;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if ((e->side == SIDE_ALIEN && (e->y >= (SCREEN_HEIGHT - e->h)) || (e->side == SIDE_ALIEN && e->y == 0)))
		{
			printf("%d\n", e->y);
			//e->y = 0;
			e->dy *= -1;
		} 

			//printf("%d\n", e->y);

		e->x += e->dx;
		e->y += e->dy;
		
			/*e->dy = -(e->dy);*/

	

		if(e != player) testVesselsCollision(e);


		/* TODO : ajouter test collision */
		if (e != player && e->x < -e->w)
		{
			e->health = 0;
		}

		if (e->health <= 0)
		{
			if (e == player)
			{
				player = NULL;
			}

			if (e == stage.fighterTail)
			{
				stage.fighterTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

void spawnEnemies(void)
{
	Entity* enemy;
	char flipCoin;

	if (--enemySpawnTimer <= 0)
	{
		enemy = malloc(sizeof(Entity));
		if(enemy) memset(enemy, 0, sizeof(Entity));
		stage.fighterTail->next = enemy;
		stage.fighterTail = enemy;

		enemy->texture = enemyTexture;
		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

		enemy->side = SIDE_ALIEN;
		enemy->health = 3;
		enemy->x = SCREEN_WIDTH;
		enemy->y = (float)(10 + (rand() % SCREEN_HEIGHT - enemy->h));
		enemy->dx = (float)(-(2 + (rand() % 4)));
		flipCoin = rand() % 2;
		enemy->dy = (float)(flipCoin ? -1.0 : 1.0);
		enemy->reload = (FPS * (1 + (rand() % 3)));
		enemy->shotMode = flipCoin;
		enemySpawnTimer = 30 + (rand() % 60); /* creates an enemy every 30 <-> 90 ms */
	}
}

static void drawFighters(void)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		blit(e->texture, e->x, e->y);
	}
}

static int testVesselsCollision(Entity* e)
{
	if (player)
	{
		if (collision(player->x, player->y, player->h, player->w, e->x, e->y, e->w, e->h))
		{
			player->health = 0;
			e->health = 0;

			return 1;
		}
		return 0;
	}
	return 0;
}


static void doEnemies(void)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if (e != player && player != NULL && --(e->reload) <= 0) fireAlienBullet(e);
	}
}

static void fireAlienBullet(Entity* e)
{
	Entity* bullet;

	bullet = malloc(sizeof(Entity));
	if (bullet)
	{
		memset(bullet, 0, sizeof(Entity));
		stage.bulletTail->next = bullet;
		stage.bulletTail = bullet;

		bullet->x = e->x + (e->w / 2);
		bullet->y = e->y + (e->h / 2);

		bullet->health = 1;
		if (e->shotMode == normal)
		{
			bullet->texture = alienBulletTexture;
			SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
			calcAzimut(player->x + (player->w / 2), player->y + (player->h / 2), bullet->x, bullet->y, &bullet->dx, &bullet->dy);
			bullet->dx *= 3 + (rand() % ALIEN_BULLET_SPEED);
			bullet->dy *= 3 + (rand() % ALIEN_BULLET_SPEED);
		}
		else
		{
			bullet->texture = megaShot;
			SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
			bullet->dx = -(ALIEN_BULLET_SPEED * 2);
			bullet->dy = 0;
		}

		bullet->side = SIDE_ALIEN;

		e->reload = (rand() % FPS * 2);

		bulletNumber++;
	}
}