#include "draw.h"


void prepareScene(void)
{
	SDL_SetRenderDrawColor(app.renderer, 171, 208, 188, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}

/* load an image and return it as a texture */
SDL_Texture* loadTexture(char* filename)
{
	SDL_Texture* texture;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[TEXTURE] Chargement de %s", filename);
	texture = IMG_LoadTexture(app.renderer, filename);
	return texture;
}

/* draws the specified texture on screen at the specified x and y coordinates.*/
void blit(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}

void blitRectScale(SDL_Texture* texture, SDL_Rect* src, int x, int y, float scale)
{
	SDL_Rect dest;

	if (scale < 0) 
	{ 
		scale = abs(scale);
	}

	dest.x = x;
	dest.y = y;
	dest.w = src->w * scale;
	dest.h = src->h * scale;

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}