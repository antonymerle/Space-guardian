#pragma once
#include "common.h"

extern void doBackground(void);
extern void doStarfield(void);
extern void drawBackground(void);
extern void drawStarfield(void);
extern void drawText(int x, int y, int r, int g, int b, double scale, int align, char* textToFormat, ...);
extern void initStage(void);
extern void initTitle(void);
extern int loadMusic(char* filename);
extern void playMusic(int loop, int volume);

extern App app;
extern Highscores highscores;