typedef struct Entity Entity;

typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} Delegate;

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS];
} App;

struct Entity {
	float x;
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int side;
	int health;
	int reload;
	SDL_Texture* texture;
	Entity* next;
};

typedef struct {
	Entity fighterHead;
	Entity* fighterTail;
	Entity bulletHead;
	Entity* bulletTail;
} Stage;