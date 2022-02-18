#pragma once
#include "common.h"

extern void cleanup(void);
extern void doInput(void);
extern void initSDL(void);
extern void initGame(void);
extern void prepareScene(void);
extern void presentScene(void);
extern void initSounds(void);
extern void initFonts(void);
extern void initHighscores(void);
extern void initTitle(void);


App app;
Stage stage;
Highscores highscores;