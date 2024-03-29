#pragma once
#include "common.h"

extern SDL_Texture* loadTexture(char* filename);
extern void blit(SDL_Texture* texture, int x, int y);
void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);
void blitRectScale(SDL_Texture* texture, SDL_Rect* src, int x, int y, double scale);
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern void calcAzimut(int srcX, int srcY, int destX, int destY, float* dx, float* dy);
extern void loadMusic(char const* filename);
extern void playMusic(int loop, int volume);
extern void playSound(int id, int channel);
extern void drawText(int x, int y, int r, int g, int b, double scale, int align, char* textToFormat, ...);

extern void doBackground(void);
extern void doStarfield(void);
extern void drawBackground(void);
extern void drawStarfield(void);
extern void initStage(void);

extern void drawText(int x, int y, int r, int g, int b, double scale, int align, char* textToFormat, ...);

extern void addHighscore(int score);
extern void initHighscores(void);

extern App app;
extern Stage stage;
extern Highscores highscores;
extern SDL_DisplayMode displayMode;