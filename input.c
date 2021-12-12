#include "input.h"

void doKeyDown(SDL_KeyboardEvent* event)
{
	/* ignore keyboard repeat events; these could queue up and cause some unexpected things to happen,
	and so we only want to deal with events where the key has been pushed down for the first time.*/
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP) app.up = 1;
		if (event->keysym.scancode == SDL_SCANCODE_DOWN) app.down = 1;
		if (event->keysym.scancode == SDL_SCANCODE_LEFT) app.left = 1;
		if (event->keysym.scancode == SDL_SCANCODE_RIGHT) app.right = 1;
	}
}

void doKeyUp(SDL_KeyboardEvent* event)
{
	/* ignore keyboard repeat events; these could queue up and cause some unexpected things to happen,
	and so we only want to deal with events where the key has been pushed down for the first time.*/
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP) app.up = 0;
		if (event->keysym.scancode == SDL_SCANCODE_DOWN) app.down = 0;
		if (event->keysym.scancode == SDL_SCANCODE_LEFT) app.left = 0;
		if (event->keysym.scancode == SDL_SCANCODE_RIGHT) app.right = 0;
	}
}

void doInput(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			doKeyDown(&event.key);
			break;
		case SDL_KEYUP:
			doKeyUp(&event.key);
			break;
		case SDL_QUIT:
			exit(0);
			break;
		default:
			break;
		}
	}
}
