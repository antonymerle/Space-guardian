#pragma once
#define SCREEN_WIDTH			1280
#define SCREEN_HEIGHT			720
#define MAX_KEYBOARD_KEYS		350

#define PLAYER_SPEED			4
#define PLAYER_BULLET_SPEED		16
#define PLAYER_MAX_HEALTH		3

#define SIDE_PLAYER				0
#define SIDE_ALIEN				1
#define SIDE_POD				2

#define MIN(a,b)				(((a)<(b))?(a):(b))
#define MAX(a,b)				(((a)>(b))?(a):(b))
#define STRNCPY(dest, src, n)	strncpy(dest, src, n); dest[n - 1] = '\0'

#define FPS						60
#define ALIEN_BULLET_SPEED		6

#define MAX_STARS				500

#define MAX_SND_CHANNELS		16

#define MAX_LINE_LENGTH			1024
#define MAX_SCORE_NAME_LENGTH   16
#define MAX_NAME_LENGTH         32			

#define NUM_HIGHSCORES			8
#define HIGHSCORES_FILE_PATH	"scores/hs.ini"

#define GLYPH_HEIGHT			28
#define GLYPH_WIDTH				18

#define L

enum
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ALIEN_FIRE,
	CH_EXPLOSION,
	CH_POINTS
};

enum
{
	SND_PLAYER_FIRE,
	SND_ALIEN_FIRE,
	SND_PLAYER_TAKE_DAMAGE,
	SND_PLAYER_DIE,
	SND_ALIEN_DIE,
	SND_POINTS,
	SND_POINT_DIE,
	SND_MAX
};

enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
};