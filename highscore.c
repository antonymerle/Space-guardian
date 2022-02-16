#include "highscore.h"

static void logic(void);
static void draw(void);
static int highscoreComparator(const void* a, const void* b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);

// read scores from file
static int parseScores(void);

static Highscore* newHighscore;
static int cursorBlink;

/* 
Parses and populate highscores table. 
If error parsing file, returns -1
If file does not exist, returns 0
If file correctly parsed, returns 1
*/
static int parseScores(void)
{
	FILE* fp;

	//const int bufferLength = MAX_SCORE_NAME_LENGTH + 1 + 10;		/* name + delim + int32 max 2'147'483'647 */
	char buffer[MAX_LINE_LENGTH];
	char* lines[NUM_HIGHSCORES];
	const char delim[2] = "\t";
	char* token;
	int i = 0;
	//Highscore highscore;


	fp = fopen(HIGHSCORES_FILE_PATH, "r");
	if (fp == NULL)
	{
		printf("Impossible d'ouvrir %s\n", HIGHSCORES_FILE_PATH);
		return 0;
	}

	memset(&highscores, 0, sizeof(Highscores));

	while (fgets(buffer, MAX_LINE_LENGTH, fp))
	{
		//printf("%s", buffer);
		//memset(&newHighscore, 0, sizeof(Highscore));
		//newHighscore->recent = 0;
		highscores.highscore[i].recent = 0;
		char* token1 = strtok(buffer, delim);
		STRNCPY(highscores.highscore[i].name, token1, MAX_SCORE_NAME_LENGTH);
		token1 = strtok(NULL, delim);


		int test = atoi(strtok(token1, delim));
		highscores.highscore[i].score = test;
		i++;
	}

	fclose(fp);
	return 1;

	// TODO : probleme de parsing � impl�menter
	//fclose(fp);
	//return -1;
}

void initHighscoreTable(void)
{
	int i;
	int code;

	code = parseScores();

	memset(&highscores, 0, sizeof(Highscores));

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		highscores.highscore[i].score = 0;
		STRNCPY(highscores.highscore[i].name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
	}
	newHighscore = NULL;
	cursorBlink = 0;
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

	if (newHighscore != NULL)
	{
		doNameInput();
	}
	else
	{
		if (app.keyboard[SDL_SCANCODE_SPACE])
		{
			initStage();
		}
	}
	if (++cursorBlink >= FPS)
	{
		cursorBlink = 0;
	}
}

static void draw(void)
{
	drawBackground();
	drawStarfield();
	if (newHighscore != NULL)
	{
		drawNameInput();
	}
	else
	{
		drawHighscores();
	}
}

static void drawHighscores(void)
{
	int i, y, r, g, b;

	y = 150;

	drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, 1, TEXT_CENTER, "HIGHSCORES");

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		r = 255;
		g = 255;
		b = 255;

		if (highscores.highscore[i].recent)
		{
			b = 0;
		}

		drawText(SCREEN_WIDTH / 2, y, r, g, b, 1, TEXT_CENTER, "#%d. %-15s ...... %03d", (i + 1), highscores.highscore[i].name, highscores.highscore[i].score);

		y += 50;
	}

	drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, 1, 1, "PRESS SPACE TO PLAY !");
}

void addHighscore(int score)
{
	Highscore newHighscores[NUM_HIGHSCORES + 1];
	int i;

	memset(newHighscores, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		newHighscores[i] = highscores.highscore[i];
		newHighscores[i].recent = 0;
	}

	newHighscores[NUM_HIGHSCORES].score = score;
	newHighscores[NUM_HIGHSCORES].recent = 1;

	qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

	newHighscore = NULL;

	for (i = 0; i < NUM_HIGHSCORES; i++)
	{
		highscores.highscore[i] = newHighscores[i];
		if (highscores.highscore[i].recent)
		{
			newHighscore = &highscores.highscore[i];
		}
	}
}

static int highscoreComparator(const void* a, const void* b)
{
	Highscore* h1 = ((Highscore*)a);
	Highscore* h2 = ((Highscore*)b);

	return h2->score - h1->score;
}

static void doNameInput(void)
{
	int i, n;
	char c;

	n = strlen(newHighscore->name);

	for (i = 0; i < strlen(app.inputText); i++)
	{
		c = toupper(app.inputText[i]);
		if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z')
		{
			newHighscore->name[n++] = c;
		}
	}

	if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		newHighscore->name[--n] = '\0';
		app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
	}

	if (app.keyboard[SDL_SCANCODE_RETURN])
	{
		if (strlen(newHighscore->name) == 0)
		{
			STRNCPY(newHighscore->name, "ANON", MAX_SCORE_NAME_LENGTH);
		}
		newHighscore = NULL;
	}
}

static void drawNameInput(void)
{
	SDL_Rect r;

	drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, 1, TEXT_CENTER, "CONGRATULATIONS, YOU REACHED A NEW HIGHSCORE !");
	drawText(SCREEN_WIDTH / 2, 120, 255, 255, 255, 1, TEXT_CENTER, "PILOT, ENTER YOUR NAME :");
	drawText(SCREEN_WIDTH / 2, 250, 128, 255, 128, 1, TEXT_CENTER, newHighscore->name);

	if (cursorBlink < FPS / 2)
	{
		r.x = ((SCREEN_WIDTH / 2) + (strlen(newHighscore->name) * GLYPH_WIDTH) / 2) + 5;
		r.y = 250;
		r.w = GLYPH_WIDTH;
		r.h = GLYPH_HEIGHT;

		SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(app.renderer, &r);
	}
	drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255, 1, TEXT_CENTER, "PRESS ENTER WHEN FINISHED");
}