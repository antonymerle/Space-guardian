#include "main.h"

static void capFramerate(unsigned int* topChrono, float* remainder);

int main(int argc, char* argv[])
{
	unsigned int topChrono;
	float remainder;

	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	initStage();

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
static void capFramerate(unsigned int* topChrono, float* remainder)
{
	unsigned int objectifTemporelPourProduireUneImageMs;		
	unsigned int intervalleDepuisDernierChrono;
	unsigned int attente;

	objectifTemporelPourProduireUneImageMs = (unsigned int)(16 + *remainder);
	*remainder -= (unsigned int)*remainder;
	intervalleDepuisDernierChrono = SDL_GetTicks() - *topChrono;
	attente = objectifTemporelPourProduireUneImageMs - intervalleDepuisDernierChrono;
	if (attente < 1) attente = 1;

	SDL_Delay(attente);

	*remainder += (float)0.667;
	*topChrono = SDL_GetTicks();
}