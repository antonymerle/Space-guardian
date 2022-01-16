#include "main.h"
//#include "stage.h"

static void capFramerate(uint32_t* topChrono, double* remainder);

int main(int argc, char* argv[])
{
	uint32_t topChrono;
	double remainder;

	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	initSounds();

	initStage();

	initFonts();

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
	uint32_t objectifTemporelPourProduireUneImageMs;
	uint32_t intervalleDepuisDernierChrono;
	/* attente pour limiter le framerate */
	uint32_t attente;

	objectifTemporelPourProduireUneImageMs = (uint32_t)(16 + *remainder);
	*remainder -= (int)*remainder;
	intervalleDepuisDernierChrono = SDL_GetTicks() - *topChrono;
	attente = objectifTemporelPourProduireUneImageMs - intervalleDepuisDernierChrono;
	if (attente < 1) attente = 1;


	SDL_Delay(attente);
	//printf("DEBUG : %u attente\n", attente);
	printf("DEBUG : %u FPS\n", objectifTemporelPourProduireUneImageMs - attente ? 1000 / (objectifTemporelPourProduireUneImageMs - attente) : 1000);

	*remainder += 0.667;
	*topChrono = SDL_GetTicks();
}
