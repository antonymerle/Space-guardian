#include "draw.h"

static SDL_Texture* getTexture(char* name);
static void addTextureToCache(char* name, SDL_Texture* sdlTexture);

void prepareScene(void)
{
	// TODO : supprimer cette couleur ?
	SDL_SetRenderDrawColor(app.renderer, 171, 208, 188, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}


/* Returns returns NULL if the texture is already cached, else returns a pointer to the texture */
static SDL_Texture* getTexture(char* name)
{
	Texture* t;

	for (t = app.textureHead.next; t != NULL; t = t->next)
	{
		if (strcmp(t->name, name) == 0)
		{
			return t->texture;
		}
	}

	return NULL;
}

static void addTextureToCache(char* name, SDL_Texture* sdlTexture)
{
	Texture* texture;

	texture = malloc(sizeof(Texture));
	if(texture) memset(texture, 0, sizeof(Texture));
	app.textureTail->next = texture;
	app.textureTail = texture;

	STRNCPY(texture->name, name, MAX_NAME_LENGTH);
	texture->texture = sdlTexture;
}

/* load an image and return it as a texture */
SDL_Texture* loadTexture(char* filename)
{
	SDL_Texture* texture;

	texture = getTexture(filename);

	if (texture == NULL)		// the texture is not already cached
	{

		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[TEXTURE] Chargement de %s", filename);

		texture = IMG_LoadTexture(app.renderer, filename);

		if (texture == NULL)
		{
			printf("Error, cannot load texture %s : %s", filename, SDL_GetError());
			exit(1);
		}

		addTextureToCache(filename, texture);
	}

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

void blitRectScale(SDL_Texture* texture, SDL_Rect* src, int x, int y, double scale)
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