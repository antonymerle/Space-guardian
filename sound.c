#include "sound.h"

static void loadSounds(void);

static Mix_Chunk* sounds[SND_MAX];
static Mix_Music* music;

void initSounds(void)
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);
	music = NULL;
	loadSounds();
}

static void loadSounds(void)
{
	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/334227__jradcoolness__laser.ogg");
	if (sounds[SND_PLAYER_FIRE] != 0)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[SON] Chargement de sound/334227__jradcoolness__laser.ogg\n");

	sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("sound/196914__dpoggioli__laser-gun.ogg");

	if (sounds[SND_ALIEN_FIRE] != 0)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[SON] Chargement de sound/196914__dpoggioli__laser-gun.ogg\n");

	sounds[SND_PLAYER_TAKE_DAMAGE] = Mix_LoadWAV("sound/270306__littlerobotsoundfactory__explosion-02.wav");

	if (sounds[SND_ALIEN_FIRE] != 0)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[SON] Chargement de sound/270306__littlerobotsoundfactory__explosion-02.wav\n");

	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sound/270328__littlerobotsoundfactory__hero-death-00.wav");

	if (sounds[SND_PLAYER_DIE] != 0)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[SON] Chargement de sound/270328__littlerobotsoundfactory__hero-death-00.wav\n");

	sounds[SND_ALIEN_DIE] = Mix_LoadWAV("sound/10 Guage Shotgun-SoundBible.com-74120584.ogg");

	if (sounds[SND_ALIEN_DIE] != 0)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[SON] Chargement de sound/10 Guage Shotgun-SoundBible.com-74120584.ogg\n");

	sounds[SND_POINTS] = Mix_LoadWAV("sound/342749__rhodesmas__notification-01.ogg");

	if (sounds[SND_POINTS] != 0)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[SON] Chargement de sound/342749__rhodesmas__notification-01.ogg");

	sounds[SND_POINT_DIE] = Mix_LoadWAV("sound/254756__jagadamba__ceramic-bell-02.wav");

	if (sounds[SND_POINT_DIE] != 0)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[SON] Chargement de sound/254756__jagadamba__ceramic-bell-02.wav");

}

void loadMusic(char const* filename)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	music = Mix_LoadMUS(filename);

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[MUSIQUE] Chargement de %s\n", filename);


}

// The volume to use from 0 to MIX_MAX_VOLUME(128).
void playMusic(int loop, int volume)
{
	Mix_VolumeMusic(volume);
	Mix_PlayMusic(music, loop ? -1 : 0);
}

void playSound(int id, int channel)
{
	switch (channel)
	{
	case CH_ALIEN_FIRE:
		Mix_Volume(channel, 60);
		Mix_PlayChannel(channel, sounds[id], 0);
		break;

	case CH_EXPLOSION:
		Mix_Volume(channel, 60);
		Mix_PlayChannel(channel, sounds[id], 0);
		break;

	default:
		Mix_Volume(channel, MIX_MAX_VOLUME);
		Mix_PlayChannel(channel, sounds[id], 0);
		break;
	}
}