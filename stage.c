#include "stage.h"

static void logic(void);
static void draw(void);
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
static void initStarField(void);

static void doExplosions(void);
static void doDebris(void);
static void addExplosions(int x, int y, int num);
static void drawDebris(void);
static void drawExplosions(void);
static void addDebris(Entity* e);
static void drawHud(void);
static void doPointsPods(void);
static void addPointsPod(int x, int y);
static void drawPointsPods(void);
static int bulletHitPoint(Entity* b);



static Entity* player;
static SDL_Texture* playerTexture;
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;
static SDL_Texture* alienBulletTexture;
static SDL_Texture* megaShot;
static SDL_Texture* explosionTexture;
static SDL_Texture* trailerTexture;
static SDL_Texture* pointTexture;


static int enemySpawnTimer;
static int stageResetTimer;

static uint8_t trailerAlpha;
static uint8_t trailerColourModifierCount = FPS;

static uint32_t highscore;
static uint32_t hudBlinkCounter;

extern uint32_t objectifTemporelPourProduireUneImageMs;
extern uint32_t attente;

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
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;
	stage.pointTail = &stage.pointHead;


	playerTexture = loadTexture("gfx/player.png");
	bulletTexture = loadTexture("gfx/playerBullet.png");
	enemyTexture = loadTexture("gfx/enemy.png");
	alienBulletTexture = loadTexture("gfx/alienBullet.png");
	megaShot = loadTexture("gfx/MegaShot.png");
	explosionTexture = loadTexture("gfx/explosion.png");
	trailerTexture = loadTexture("gfx/trailerPlayer.png");
	pointTexture = loadTexture("gfx/points.png");

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

	//loadMusic("music/Mercury.ogg");
	//playMusic(1);
	resetStage();
	stage.score = 0;
	initPlayer();

	enemySpawnTimer = 0;
	stageResetTimer = FPS * 3;

	// DEBUG
	bulletNumber = 0;
	hitCount = 0;
}

static void resetStage(void)
{
	Entity* e;
	Entity* p;
	Explosion* ex;
	Debris* d;

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

	while (stage.explosionHead.next)
	{
		ex = stage.explosionHead.next;
		stage.explosionHead.next = ex->next;
		free(ex);
	}

	while (stage.debrisHead.next)
	{
		d = stage.debrisHead.next;
		stage.debrisHead.next = d->next;
		free(d);
	}

	while (stage.pointHead.next)
	{
		p = stage.pointHead.next;
		stage.pointHead.next = p->next;
		free(p);
	}

	memset(&stage, 0, sizeof(Stage));
	stage.score = 0;								/* Est-ce bien nécessaire ? cf memset*/
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;
	stage.pointTail = &stage.pointHead;
}

static void initPlayer(void)
{
	player = malloc(sizeof(Entity));
	if (player) memset(player, 0, sizeof(Entity));

	stage.fighterTail->next = player;
	stage.fighterTail = player;

	player->health = PLAYER_MAX_HEALTH;
	player->side = SIDE_PLAYER;
	player->x = 100;
	player->y = 100;

	player->texture = playerTexture;
	player->trailer = trailerTexture;

	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

}

static void logic(void)
{
	doBackground();
	doStarfield();
	doPlayer();
	doEnemies();
	doFighters();
	doBullets();
	doExplosions();
	doDebris();
	doPointsPods();
	spawnEnemies();
	cadrePlayer();
	if (player == NULL && --stageResetTimer <= 0)
	{
		if(stage.score > highscores.currentHighscore)
			addHighscore(stage.score);
		initHighscores();
	}
}

static void doPlayer(void)
{
	int r, g, b;

	if (player)
	{
		player->dx = 0;
		player->dy = 0;

		// TODO make a trailerAlpha function
		r = g = b = 0;
		if (--trailerColourModifierCount == 0)
		{
			switch (rand() % 3)
			{
			case 0:
				r = 255;
				g = 128;
				break;
			case 1:
				r = 255;
				g = 255;
				break;
			default:
				r = 255;
				g = 255;
				b = 255;
				break;
			}

			SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
			SDL_SetTextureBlendMode(trailerTexture, SDL_BLENDMODE_ADD);
			SDL_SetTextureColorMod(trailerTexture, r, g, b);
			SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);

			trailerColourModifierCount = 4;
		}


		if (trailerAlpha > 0 && (!app.keyboard[SDL_SCANCODE_RIGHT] || !app.keyboard[SDL_SCANCODE_UP] || app.keyboard[SDL_SCANCODE_DOWN])) trailerAlpha -= 5;

		SDL_SetTextureAlphaMod(player->trailer, trailerAlpha);

		if (player->reload > 0) player->reload--;
		if (app.keyboard[SDL_SCANCODE_UP])
		{
			player->dy = -PLAYER_SPEED;
			if (trailerAlpha <= SDL_MAX_UINT8 - 10) trailerAlpha += 10;
		}
		if (app.keyboard[SDL_SCANCODE_DOWN])
		{
			player->dy = PLAYER_SPEED;
			if (trailerAlpha <= SDL_MAX_UINT8 - 10) trailerAlpha += 10;
		}
		if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
		if (app.keyboard[SDL_SCANCODE_RIGHT])
		{
			player->dx = PLAYER_SPEED;
			if (trailerAlpha <= SDL_MAX_UINT8 - 10) trailerAlpha += 10;
		}
		if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
		{
			fireBullet();
			playSound(SND_PLAYER_FIRE, CH_PLAYER);
		}
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

		if (bulletHitFighter(b) || bulletHitPoint(b) || b->x > SCREEN_WIDTH || b->x <= 0 || b->y > SCREEN_HEIGHT || b->y <= 0 || (b->dx == 0) && (b->dy == 0))
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
		if (e->side != b->side
			&& collision(e->x, e->y, e->w, e->h, b->x, b->y, b->w, b->h))
		{
			hitCount++;
			b->health = 0;
			e->health--;

			if (e == player)
			{
				if (player->health <= 0)
				{
					playSound(SND_PLAYER_DIE, CH_PLAYER);
				}
				else
				{
					playSound(SND_PLAYER_TAKE_DAMAGE, CH_PLAYER);
				}
			}
			else
			{
				if(e->x % 2) addPointsPod(e->x + e->w / 2, e->y + e->h / 2);
				playSound(SND_ALIEN_DIE, CH_EXPLOSION);
				// TODO : correct bug : increment score when alien ship die, not when it's hit
				//stage.score++;
				//highscore = MAX(stage.score, highscore);
			}

			return 1;
		}
	}

	return 0;
}



static void draw(void)
{
	drawBackground();
	drawStarfield();
	drawPointsPods();
	drawFighters();
	drawDebris();
	drawExplosions();
	drawBullets();
	drawHud();
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
	/* Initializes random number generator */
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
			e->dy *= -1;
		}

		e->x += e->dx;
		e->y += e->dy;

		if (e != player) testVesselsCollision(e);

		if (e != player && e->x < -e->w)
		{
			e->health = 0;
		}

		if (e->health <= 0)
		{
			if (e == player)
			{
				addDebris(e);
				addExplosions(e->x, e->y, 32);
				player = NULL;
			}

			if (e == stage.fighterTail)
			{
				stage.fighterTail = prev;
			}

			if (e->x > 0)
			{
				addDebris(e);
				addExplosions(e->x, e->y, 32);
				if (e->side == SIDE_ALIEN)
					stage.score++;
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
		if (enemy) memset(enemy, 0, sizeof(Entity));
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
		blit(e->trailer, e->x - ((e->w / 2) + 8), e->y - 5);
		blit(e->trailer, e->x - ((e->w / 2) + 8), e->y + 7);
		blit(e->trailer, e->x - ((e->w / 2) + 8), e->y + (e->h / 2) + 2);
		blit(e->trailer, e->x - ((e->w / 2) + 8), e->y + (e->h / 2) + 14);
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
		if (e != player && player != NULL && --(e->reload) <= 0)
		{
			fireAlienBullet(e);
			playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
		}
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


static void doExplosions(void)
{
	Explosion* e;
	Explosion* prev;

	prev = &stage.explosionHead;

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		if (--e->a <= 0)
		{
			if (e == stage.explosionTail)
			{
				stage.explosionTail = prev;
			}
			prev->next = e->next;
			free(e);
			e = prev;
		}
		prev = e;
	}
}

static void doDebris(void)
{
	Debris* d;
	Debris* prev;

	prev = &stage.debrisHead;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		d->x += d->dx;
		d->y += d->dy;

		d->dy += 0.5;

		if (--d->life <= 0)
		{
			if (d == stage.debrisTail) stage.debrisTail = prev;
			prev->next = d->next;
			free(d);
			d = prev;
		}
		prev = d;
	}
}

static void addExplosions(int x, int y, int num)
{
	Explosion* e;
	int i;

	for (i = 0; i < num; i++)
	{
		e = malloc(sizeof(Explosion));
		if (e) memset(e, 0, sizeof(Explosion));
		stage.explosionTail->next = e;
		stage.explosionTail = e;

		e->x = x + (rand() % 32) - (rand() % 32);
		e->y = y + (rand() % 32) - (rand() % 32);
		e->dx = (rand() % 10) - (rand() % 10);
		e->dy = (rand() % 10) - (rand() % 10);

		e->dx /= 10;
		e->dy /= 10;

		switch (rand() % 4)
		{
		case 0:
			e->r = 255;
			break;
		case 1:
			e->r = 255;
			e->g = 128;
			break;
		case 2:
			e->r = 255;
			e->g = 255;
			break;
		default:
			e->r = 255;
			e->g = 255;
			e->b = 255;
			break;
		}
		e->a = rand() % FPS - 3;
	}
}

static void addDebris(Entity* e)
{
	Debris* d;
	int x;
	int y;
	int w;
	int h;

	w = e->w / 2;
	h = e->h / 2;

	for (y = 0; y <= h; y += h)
	{
		for (x = 0; x <= w; x += w)
		{
			d = malloc(sizeof(Debris));
			if (d) memset(d, 0, sizeof(Debris));
			stage.debrisTail->next = d;
			stage.debrisTail = d;

			d->x = e->x + e->w / 2;
			d->y = e->y + e->h / 2;
			d->dx = (rand() % 5) - (rand() % 5);
			d->dy = -(5 + (rand() % 12));
			d->life = FPS * 2;
			d->texture = e->texture;

			d->rect.x = x;
			d->rect.y = y;
			d->rect.w = w;
			d->rect.h = h;
		}
	}
}

static void drawDebris(void)
{
	Debris* d;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		blitRect(d->texture, &d->rect, d->x, d->y);
	}
}

static void drawExplosions(void)
{
	Explosion* e;

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{
		SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
		SDL_SetTextureAlphaMod(explosionTexture, e->a);

		blit(explosionTexture, e->x, e->y);
	}
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

static void drawHud(void)
{
	double healthRatio;

	drawText(10, 10, 255, 255, 255, 0.5, TEXT_LEFT, "SCORE: %03d", stage.score);
	drawText(10, SCREEN_HEIGHT - 50, 255, 255, 255, 0.5, TEXT_LEFT, "FPS : %u", objectifTemporelPourProduireUneImageMs - attente ? 1000 / (objectifTemporelPourProduireUneImageMs - attente) : 1000);

	if (stage.score < highscores.highscore[0].score)
	{
		drawText(SCREEN_WIDTH - 10, 10, 0, 255, 0, 0.5, TEXT_RIGHT, "HIGH SCORE: %03d", highscores.highscore[0].score);
	}
	else
	{
		drawText(SCREEN_WIDTH - 10, 10, 0, 255, 0, 0.5, TEXT_RIGHT, "HIGH SCORE: %03d", stage.score);
	}

	if (player)
	{
		healthRatio = ((double)(player->health) / (double)PLAYER_MAX_HEALTH) * 100.0;

		if (healthRatio == 100)
		{
			drawText(10, 40, 0, 255, 0, 0.5, TEXT_LEFT, "HEALTH: %3.0f \%", healthRatio);
		}
		else if (healthRatio >= 34 && healthRatio <= 67)
		{
			drawText(10, 40, 255, 128, 0, 0.5, TEXT_LEFT, "HEALTH: %3.0f \%", healthRatio);
		}
		else
		{
			hudBlinkCounter++;
			if (hudBlinkCounter < FPS)
			{
				drawText(10, 40, 255, 0, 0, 0.5, TEXT_LEFT, "HEALTH: %3.0f \%", healthRatio);
			}
			if (hudBlinkCounter > FPS * 2) hudBlinkCounter = 0;
		}
	}

}

static int bulletHitPoint(Entity* b)
{
	Entity* e;
	for (e = stage.pointHead.next; e != NULL; e = e->next)
	{
		if (collision(e->x, e->y, e->w, e->h, b->x, b->y, b->w, b->h))
		{
			b->health = 0;
			e->health = 0;
			playSound(SND_POINT_DIE, CH_POINTS);
		return 1;
		}
	}
	return 0;
}

static void doPointsPods(void)
{
	Entity* e;
	Entity* prev;

	prev = &stage.pointHead;

	for (e = stage.pointHead.next; e != NULL; e = e->next)
	{

		if (e->x < 0)
		{
			e->x = 0;
			e->dx = -e->dx;
		}

		if (e->x + e->w > SCREEN_WIDTH)
		{
			e->x = SCREEN_WIDTH - e->w;
			e->dx = -e->dx;
		}

		if (e->y < 0)
		{
			e->y = 0;
			e->dy = -e->dy;
		}

		if (e->y + e->h > SCREEN_HEIGHT)
		{
			e->y = SCREEN_HEIGHT - e->h;
			e->dy = -e->dy;
		}

		e->x += e->dx;
		e->y += e->dy;

		if (player != NULL && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h))
		{
			e->health = 0;
			if (player->health < PLAYER_MAX_HEALTH) player->health++;
			playSound(SND_POINTS, CH_POINTS);
		}

		if (--e->health <= 0)
		{
			if (e == stage.pointTail)
			{
				stage.pointTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}
		prev = e;
	}
}

static void addPointsPod(int x, int y)
{
	Entity* e;

	e = malloc(sizeof(Entity));
	if (e != NULL) memset(e, 0, sizeof(Entity));

	stage.pointTail->next = e;
	stage.pointTail = e;

	e->side = SIDE_POD;

	e->x = x;
	e->y = y;

	e->dx = -(rand() % 5);
	e->dy = (rand() % 5 - rand() % 5);

	SDL_QueryTexture(pointTexture, NULL, NULL, &e->w, &e->h);

	e->x -= e->w / 2;
	e->y -= e->h / 2;

	e->health = FPS * 10;
	e->texture = pointTexture;
}

static void drawPointsPods(void)
{
	Entity* e;

	for (e = stage.pointHead.next; e != NULL; e = e->next)
	{
		blit(pointTexture, e->x, e->y);
	}
}