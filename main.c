#include "main.h"

int main(int argc, char* argv[])
{
	memset(&app, 0, sizeof(App));
	memset(&player, 0, sizeof(Entity));
	memset(&bullet, 0, sizeof(Entity));

	initSDL();

	player.x = 100;
	player.y = 100;

	player.texture = loadTexture("gfx/player.png");
	bullet.texture = loadTexture("gfx/playerBullet.png");

	/* Just to check textures dimensions to block ship at screen limits */
	SDL_Rect playerTexture = { 0 };
	SDL_QueryTexture(player.texture, NULL, NULL, &playerTexture.w, &playerTexture.h);

	atexit(cleanup);

	while (1)
	{
		prepareScene();
		doInput();

		player.x += player.dx;
		player.y += player.dy;

		if (app.up && player.y > 0) player.y -= 4;
		if (app.down && player.y < SCREEN_HEIGHT - playerTexture.h) player.y += 4;
		if (app.left && player.x > 0) player.x -= 4;
		if (app.right && player.x < SCREEN_WIDTH - playerTexture.w) player.x += 4;

		if (app.fire && bullet.health == 0)
		{
			bullet.x = player.x + playerTexture.w / 2;
			bullet.y = player.y + playerTexture.h / 2;
			bullet.dx = 32;
			bullet.dy = 4;
			bullet.health = 1;
		}

		bullet.x += bullet.dx;
		bullet.y += bullet.dy;

		if (bullet.x > SCREEN_WIDTH) bullet.health = 0;

		blit(player.texture, player.x, player.y);
		if (bullet.health > 0) blit(bullet.texture, bullet.x, bullet.y);

		presentScene();
		SDL_Delay(16);
	}

	return 0;
}
