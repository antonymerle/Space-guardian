#include "util.h"

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}

/*
Calculate the normalized step from one point to another.
The dx or dy will always be 1 (or -1), while the other could be any value between -1 and 1.
This will be used to tell the alien's bullets which way they need to go to reach their target when they're fired.
*/
void calcAzimut(int srcX, int srcY, int destX, int destY, float* dx, float* dy)
{
	int steps = MAX(abs(srcX - destX), abs(srcY - destY));

	if (steps == 0)
	{
		*dx = 0;
		*dy = 0;
		return;
	}

	*dx = srcX - destX;
	*dx /= steps;

	*dy = srcY - destY;
	*dy /= steps;
}