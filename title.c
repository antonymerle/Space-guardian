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
	//memset(&app.delegate.logic, 0, sizeof(app.delegate.logic));
	//memset(&app.delegate.draw, 0, sizeof(app.delegate.draw));

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

	if (revealH < displayMode.h)
	{
		revealH++;
	}

	if (revealW < displayMode.w)
	{
		revealW += 6;
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
		drawText(displayMode.w / 2, ((displayMode.h / 6) + SPRITE_TITLE_HEIGHT + 100), 255, 255, 255, 1, TEXT_CENTER, "PRESS SPACE TO PLAY!");
	}

	drawText(displayMode.w / 2, displayMode.h - 50, 255, 255, 255, 0.5, TEXT_CENTER, "ANTONY MERLE, 2022");

}

static void drawTitle(void)
{
	//SDL_Rect srcRect = { spriteTitleIndex * SPRITE_TITLE_WIDTH, 0, SPRITE_TITLE_WIDTH, SPRITE_TITLE_HEIGHT };
	SDL_Rect srcRect;

	srcRect.x = 0;
	srcRect.y = 0;

	SDL_QueryTexture(titleTexture, NULL, NULL, &srcRect.w, &srcRect.h);

	//srcRect.w = MIN(revealW, srcRect.w);
	//srcRect.h = MIN(revealH, srcRect.h);

	blitRect(titleTexture, &srcRect, (displayMode.w / 2) - (SPRITE_TITLE_WIDTH / 2), displayMode.h / 6);

	//animationCounter++;


	//if (animationCounter % 128 == 0 && revealW > displayMode.w - 8)
	//{
	//	spriteTitleIndex++;
	//	if (spriteTitleIndex > 5)
	//	{
	//		spriteTitleIndex = 5;
	//	}
	//}
}