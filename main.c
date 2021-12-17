#include "main.h"

static void capFramerate(Uint64* then, float* remainder);

int main(int argc, char* argv[])
{
	Uint64 then;
	float remainder;

	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	initStage();

	then = SDL_GetTicks64();
	remainder = 0;

	while (1)
	{
		prepareScene();
		doInput();
		app.delegate.logic();
		app.delegate.draw();

		presentScene();
		capFramerate(&then, &remainder);
	}

	return 0;
}

static void capFramerate(Uint64* then, float* remainder)
{
	Uint64 wait;
	Uint64 frameTime;

	wait = 16 + *remainder;
	*remainder -= (Uint64)*remainder;
	frameTime = SDL_GetTicks64() - *then;
	wait -= frameTime;
	if (wait < 1) wait = 1;
	SDL_Delay(wait);
	*remainder += 0.667;
	*then = SDL_GetTicks64();
}