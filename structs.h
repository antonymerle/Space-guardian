typedef struct Entity Entity;
typedef enum { normal, powerful } ShotMode;

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
	int x;
	int y;
	int w;
	int h;
	float dx;
	float dy;
	int side;
	int health;
	int reload;
	ShotMode shotMode;
	SDL_Texture* texture;
	Entity* next;
};

typedef struct {
	Entity fighterHead;
	Entity* fighterTail;
	Entity bulletHead;
	Entity* bulletTail;
} Stage;