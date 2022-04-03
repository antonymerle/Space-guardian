#include "input.h"

/*
On ignore les events répétés du clavier qui pourraient s'embouteiller et créer de l'UB.
On ne retient que les events où la touche a été pressée pour la 1ere fois.
*/
void doKeyDown(SDL_KeyboardEvent* event)
{
	if (event->repeat == 0 && event->keysym.scancode >= 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[event->keysym.scancode] = 1;
	}
}


void doKeyUp(SDL_KeyboardEvent* event)
{
	if (event->repeat == 0 && event->keysym.scancode >= 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[event->keysym.scancode] = 0;
	}
}

void doInput(void)
{
	SDL_Event event;

	memset(app.inputText, '\0', MAX_LINE_LENGTH);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			doKeyDown(&event.key);
            if (app.keyboard[SDL_SCANCODE_ESCAPE]) exit(0);
			break;

		case SDL_KEYUP:
			doKeyUp(&event.key);
			break;

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_TEXTINPUT:
			STRNCPY(app.inputText, event.text.text, MAX_LINE_LENGTH);
			break;

		default:
			printf("Unhandled event\n");
			break;
		}
	}
}
