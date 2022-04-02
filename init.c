#include "init.h"

void initSDL(void)
{
	int rendererFlags;
	int windowFlags;
	int i;

	rendererFlags = SDL_RENDERER_ACCELERATED;
	windowFlags = 0;


	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Impossible d'initialiser SDL : %s\n", SDL_GetError());
		exit(1);
	}

	for (i = 0; i < SDL_GetNumVideoDisplays(); ++i) {

		int should_be_zero = SDL_GetCurrentDisplayMode(i, &displayMode);

		if (should_be_zero != 0)
			// In case of error...
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

		else
			// On success, print the current display mode.
			SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, displayMode.w, displayMode.h, displayMode.refresh_rate);

	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Impossible d'initialiser SDL_mixer : %s\n", SDL_GetError());
		exit(1);
	}

	Mix_AllocateChannels(MAX_SND_CHANNELS);
	

	app.window = SDL_CreateWindow("Space Guardian", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayMode.w, displayMode.h, SDL_WINDOW_FULLSCREEN_DESKTOP);

	if (!app.window)
	{
		printf("Echec de création de la fenêtre %d * %d : %s\n", displayMode.w, displayMode.h, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

	if (!app.renderer)
	{
		printf("Echec de création du renderer : %s\n", SDL_GetError());
		exit(1);
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	SDL_ShowCursor(0);

}

void initGame(void)
{
	initBackground();
	initStarfield();
	initSounds();
	initFonts();
	initHighscoreTable();
	memset(&stage, 0, sizeof(Stage));
	loadMusic("music/title-theme.opus");
	playMusic(1, 128);
}


void cleanup(void)
{
	SDL_DestroyRenderer(app.renderer);

	SDL_DestroyWindow(app.window);

	SDL_Quit();
}
