#pragma once
#include "common.h"

extern void cleanup(void);
extern void doInput(void);
extern void initSDL(void);
extern void initStage(void);
extern void prepareScene(void);
extern void presentScene(void);
extern void initSounds(void);
extern void initFonts(void);


App app;
Stage stage;