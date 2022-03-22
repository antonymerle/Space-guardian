#include "background.h"

static Star stars[MAX_STARS];
static int backgroundX;
static SDL_Texture* background;


void initBackground(void)
{
	background = loadTexture("gfx/BlueNebula-1-512x512.png");
	backgroundX = 0;
}


void initStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = 1 + rand() % 8;
	}
}


void doBackground(void)
{
	int w;
	SDL_QueryTexture(background, NULL, NULL, &w, NULL);
	if (--backgroundX < -w) backgroundX = 0;
}


void doStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x -= stars[i].speed;
		if (stars[i].x < 0)
		{
			stars[i].x += SCREEN_WIDTH + stars[i].x;
		}
	}
}


void drawBackground(void)
{
	SDL_Rect dest;
	int x;
	int y;

	SDL_QueryTexture(background, NULL, NULL, &dest.w, &dest.h);

	for (x = backgroundX; x < SCREEN_WIDTH; x += dest.w)
	{
		for (y = 0; y < SCREEN_HEIGHT; y += dest.h)
		{
			dest.x = x;
			dest.y = y;
			SDL_RenderCopy(app.renderer, background, NULL, &dest);
		}
	}
}


void drawStarfield(void)
{
	int i;
	int c;

	for (i = 0; i < MAX_STARS; i++)
	{
		c = 32 * stars[i].speed;
		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);
		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}