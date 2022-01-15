#include "text.h"

static SDL_Texture* fontTexture;
static char drawTextBuffer[MAX_STR_LENGTH];

void initFonts(void)
{
	fontTexture = loadTexture("gfx/font.png");
}

void drawText(int x, int y, int r, int g, int b, double scale, char* textToFormat, ...)
{
	int i, len, c;
	SDL_Rect rect;				/* to specify what region of the texture to use */
	va_list args;

	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, textToFormat);
	vsprintf(drawTextBuffer, textToFormat, args);
	va_end(args);

	len = strlen(drawTextBuffer);

	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;

	SDL_SetTextureColorMod(fontTexture, r, g, b);

	for (i = 0; i < len; i++)
	{
		c = drawTextBuffer[i];

		if (c >= ' ' && c <= 'Z')
		{
			/* This is because a space is the first character in font texture
			and therefore has an x position of 0. */
			rect.x = (c - ' ') * GLYPH_WIDTH;
			blitRectScale(fontTexture, &rect, x, y, scale);

			x += GLYPH_WIDTH;
		}
	}

}