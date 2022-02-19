#include "highscore.h"

static void logic(void);
static void draw(void);
static int highscoreComparator(const void* a, const void* b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);
static int isWellFormattedLine(const char* str);
static int getCurrentMinHighscore(void);
static int parseScores(void);
static int writeScores(void);

static int timeout;
static Highscore* newHighscore;

static int getCurrentMinHighscore(void)
{

	int minScore = 1<<32 - 1;

	for (size_t i = 0; i < NUM_HIGHSCORES; i++)
	{
		if (highscores.highscore[i].score < minScore && highscores.highscore[i].score > 0)
		{
			minScore = highscores.highscore[i].score;
		}
	}

	return minScore;
}


static int writeScores(void)
{
	FILE* fp;
	char line[MAX_LINE_LENGTH];

	fp = fopen(HIGHSCORES_FILE_PATH, "w");
	if (fp == NULL)
	{
		printf("Impossible d'ouvrir %s\n", HIGHSCORES_FILE_PATH);
		return 1;
	}

	for (size_t i = 0; i < NUM_HIGHSCORES; i++)
	{
		sprintf(line, "%s\t%03d\n", highscores.highscore[i].name, highscores.highscore[i].score);
		fputs(line, fp);
		memset(line, '\0', sizeof(MAX_LINE_LENGTH));
	}

	fclose(fp);
	return 0;
}


static Highscore* newHighscore;
static int cursorBlink;

/* 
Parses and populate highscores table. 
If file correctly parsed, returns 0
If file does not exist, returns -1
If error parsing file, returns 1
*/
static int parseScores(void)
{
	FILE* fp;
	char buffer[MAX_LINE_LENGTH];
	const char delim[2] = "\t";

	memset(&highscores, 0, sizeof(Highscores));

	fp = fopen(HIGHSCORES_FILE_PATH, "r");
	if (fp == NULL)
	{
		printf("Impossible d'ouvrir %s\n", HIGHSCORES_FILE_PATH);
		return -1;
	}

	for (size_t i = 0; i < NUM_HIGHSCORES; i++)
	{
		if(!(fgets(buffer, MAX_LINE_LENGTH, fp)))
			break;
		if (isWellFormattedLine(buffer) == 0)
			return 1;
		highscores.highscore[i].recent = 0;
		STRNCPY(highscores.highscore[i].name, strtok(buffer, delim), MAX_SCORE_NAME_LENGTH);
		highscores.highscore[i].score = atoi(strtok(NULL, delim));
	}

	for (size_t i = 0; i < NUM_HIGHSCORES; i++)
	{
		if (highscores.highscore[i].score == 0)
		{
			STRNCPY(highscores.highscore[i].name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
		}
	}

	qsort(highscores.highscore, NUM_HIGHSCORES, sizeof(Highscore), highscoreComparator);

	highscores.currentMinHighscore = getCurrentMinHighscore();

	fclose(fp);
	return 0;
}

void initHighscoreTable(void)
{
	int i;
	int code;

	code = parseScores();
	
	if (code != 0)
	{
		for (i = 0; i < NUM_HIGHSCORES; i++)
		{
			highscores.highscore[i].score = 0;
			STRNCPY(highscores.highscore[i].name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
		}
	}

	newHighscore = NULL;
	cursorBlink = 0;
}

void initHighscores(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

	timeout = FPS * 10;
}

static void logic(void)
{
	doBackground();
	doStarfield();

	if (newHighscore != NULL)
	{
		doNameInput();
		writeScores();
		highscores.currentMinHighscore = getCurrentMinHighscore();
	}
	else
	{
		if (--timeout <= 0)
		{
			initTitle();
		}
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

		if (timeout % 40 < 20)
		{
			drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, 1, TEXT_CENTER, "PRESS SPACE TO PLAY !");
		}
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

	if (timeout % 40 < 20)
	{
		drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, 1, 1, "PRESS SPACE TO PLAY !");
	}
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

static int isWellFormattedLine(const char* str)
{
	char* c = str;
	size_t counter = 0;
	int isTab = 0;

	while (*c != '\n' && *c != '\0')
	{
		while (*c != '\t' && isTab == 0)			/* check name */
		{
			if (*c < ' ' && *c > 'Z')
			{
				return 0;
			}
			c++;
		}

		if (*c == '\t' && isTab == 0)				/* pass tab delimiter */
		{
			if (counter == 0)						/* name is void, malformed */
				return 0;
			c++;
			isTab = 1;
		}

		if (*c < '0' || *c > '9')					/* check score */
		{
			return 0;
		}
		c++;
		counter++;
	}
	return 1;
}