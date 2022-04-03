#include "main.h"
//#include "stage.h"

static void capFramerate(uint32_t* topChrono, double* remainder);

//uint32_t objectifTemporelPourProduireUneImageMs;
//uint32_t intervalleDepuisDernierChrono;
//uint32_t attente;

int main(int argc, char* argv[])
{
	uint32_t topChrono;
	double remainder;

	memset(&app, 0, sizeof(App));
	app.textureTail = &app.textureHead;

	initSDL();

	atexit(cleanup);

	initGame();
	initTitle();
	//initHighscores();

	topChrono = SDL_GetTicks();
	remainder = 0;

	while (1)
	{

		prepareScene();
		doInput();
		app.delegate.logic();
		app.delegate.draw();

		presentScene();
		capFramerate(&topChrono, &remainder);
	}

	return 0;
}

/*
* 1000 ms / 60 = 16.6667 ms
* si nécessaire, attend jusqu'à 16.667 ms pour produire une image,
* avec comme objectif 60 images/s
*/
static void capFramerate(uint32_t* topChrono, double* remainder)
{
	uint32_t attente;
	uint32_t intervalleDepuisDernierChrono;
	uint32_t objectifTemporelPourProduireUneImageMs;

	objectifTemporelPourProduireUneImageMs = (uint32_t)(16 + *remainder);
	*remainder -= (int)*remainder;
	intervalleDepuisDernierChrono = SDL_GetTicks() - *topChrono;
	attente = objectifTemporelPourProduireUneImageMs - intervalleDepuisDernierChrono;
	if (attente < 1) attente = 1;
	if (attente > 16) attente = 16;

	/* attente pour limiter le framerate */
	SDL_Delay(attente);
#if DEBUG
	printf("DEBUG : %u FPS\n", objectifTemporelPourProduireUneImageMs - attente ? 1000 / (objectifTemporelPourProduireUneImageMs - attente) : 1000);
#endif
	*remainder += 0.667;
	*topChrono = SDL_GetTicks();
}
