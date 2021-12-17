#include "common.h"

extern SDL_Texture* loadTexture(char* filename);
extern void blit(SDL_Texture* texture, int x, int y);

App app;
Stage stage;