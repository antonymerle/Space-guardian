#include "common.h"

extern SDL_Texture* loadTexture(char* filename);
extern void blit(SDL_Texture* texture, int x, int y);
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern void calcAzimut(int srcX, int srcY, int destX, int destY, float* dx, float* dy);


extern App app;
extern Stage stage;