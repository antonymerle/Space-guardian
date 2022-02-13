#include "highscore.h"

static void logic(void);
static void draw(void);
static int highscoreComparator(const void* a, const void* b);
static void drawHighscores(void);

void initHighscoreTable(void)
{
	int i;

	memset(&highscores, 0, sizeof(Highscores));

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		highscores.highscore[i].score = NUM_HIGHSCORES - i;
	}
}

void initHighscores(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
}

static void logic(void)
{
	doBackground();
	doStarfield();

	if (app.keyboard[SDL_SCANCODE_LCTRL])
	{
		initStage();
	}
}

static void draw(void)
{
	drawBackground();
	drawStarfield();
	drawHighscores();
}

static void drawHighscores(void)
{
	int i;
	int y;

	y = 150;

	drawText(425, 70, 255, 255, 255, 1, "HIGHSCORES");

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		if (highscores.highscore[i].recent)
		{
			drawText(425, y, 255, 255, 0, 1, "#%d ............. %03d", (i + 1), highscores.highscore[i].score);
		}
		else
		{
			drawText(425, y, 255, 255, 255, 1, "#%d ............. %03d", (i + 1), highscores.highscore[i].score);
		}

		y += 50;
	}

	drawText(425, 600, 255, 255, 255, 1, "PRESS FIRE TO PLAY !");
}

void addHighscore(int score)
{
	Highscore newHighscore[NUM_HIGHSCORES + 1];
	int i;

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		newHighscore[i] = highscores.highscore[i];
		newHighscore[i].recent = 0;
	}

	newHighscore[NUM_HIGHSCORES].score = score;
	newHighscore[NUM_HIGHSCORES].recent = 1;

	qsort(newHighscore, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		highscores.highscore[i] = newHighscore[i];
	}
}

static int highscoreComparator(const void* a, const void* b)
{
	Highscore* h1 = ((Highscore*)a);
	Highscore* h2 = ((Highscore*)b);

	return h2->score - h1->score;
}