#include "title.h"

static void logic(void);
static void draw(void);
static void drawTitle(void);

static SDL_Texture* titleTexture;

static int revealH = 200;
static int revealW = 200;
static int timeout;
static uint8_t animationCounter;
static size_t spriteTitleIndex;

void initTitle(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

	titleTexture = loadTexture("gfx/title.png");
	
	timeout = FPS * 60;
}

static void logic(void)
{
	doBackground();
	doStarfield();

	if (revealH < SCREEN_HEIGHT)
	{
		revealH++;
	}

	if (revealW < SCREEN_WIDTH)
	{
		revealW += 4;
	}

	if (--timeout <= 0)
	{
		initHighscores();
	}

	if (app.keyboard[SDL_SCANCODE_SPACE])
	{
		initStage();
	}
}

static void draw(void)
{
	drawBackground();
	drawStarfield();
	drawTitle();

	if (timeout % 40 < 20)						// texte clignote
	{
		drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, 1, TEXT_CENTER, "PRESS SPACE TO PLAY!");
	}
}

static void drawTitle(void)
{
	//SDL_Rect r;

	SDL_Rect srcRect = { spriteTitleIndex * SPRITE_TITLE_WIDTH, 0, SPRITE_TITLE_WIDTH, SPRITE_TITLE_HEIGHT };


	//r.x = 0;
	//r.y = 0;

	//SDL_QueryTexture(titleTexture, NULL, NULL, &r.w, &r.h);

	srcRect.w = MIN(revealW, srcRect.w);
	srcRect.h = MIN(revealH, srcRect.h);

	blitRect(titleTexture, &srcRect, (SCREEN_WIDTH / 2) - (SPRITE_TITLE_WIDTH / 2), 100);

	animationCounter++;


	if (animationCounter % 128 == 0 && revealW > SCREEN_WIDTH - 8)
	{
		spriteTitleIndex++;
		if (spriteTitleIndex > 7)
		{
			spriteTitleIndex = 7;
		}
	}
}