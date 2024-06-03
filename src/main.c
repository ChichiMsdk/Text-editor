#include "SDL3/SDL_audio.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "editor.h"

SDL_Surface *
SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth,
		int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
    return SDL_CreateSurfaceFrom(pixels, width, height, pitch,
            SDL_GetPixelFormatEnumForMasks (depth, Rmask, Gmask, Bmask, Amask));
}

int			volume;
int			volume1;
Mix_Music	*opp;
int			buff_end;
static 		YUinstance inst;
int 		running = 1;
char 		buff_txt[BUFF_MAX];
float 		textWidth;
float 		textHeight;
int 		WINDOW_WIDTH = 1600;
int 		WINDOW_HEIGHT = 1200;
TTF_Font 	*font;
Font 		font1;
Font 		font2;
sfx			sfx_key[7];
Mix_Chunk	*sfx_autre;
int			sfx_index;

void 
init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(1);
	}
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "%s\n", TTF_GetError());
		SDL_Quit();
		exit(1);
	}
	if (Mix_Init(MIX_INIT_WAVPACK | MIX_INIT_FLAC | MIX_INIT_MP3) == -1)
	{
		fprintf(stderr, "%s\n", Mix_GetError());
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}

	if (Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, NULL) == -1)
	{
		fprintf(stderr, "%s\n", Mix_GetError());
		TTF_Quit();
		Mix_Quit();
		SDL_Quit();
		exit(1);
	}

	inst.window = SDL_CreateWindow("Chichi Editor", WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (inst.window == NULL)
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}

	inst.renderer = SDL_CreateRenderer(inst.window,NULL);
	if (inst.renderer == NULL)
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_DestroyWindow(inst.window);
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}

	/*TODO: Write function to change font */

	char *path_font = 
		"E:\\Downloads\\installers\\4coder\\fonts\\liberation-mono.ttf";

	int size_font = 1;
	font = TTF_OpenFont(path_font, size_font);
	if (inst.renderer == NULL)
	{
		fprintf(stderr, "%s\n", TTF_GetError());
		SDL_DestroyRenderer(inst.renderer);
		SDL_DestroyWindow(inst.window);
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}
	if (TTF_SetFontSize(font, 100) != 0)
	{
		fprintf(stderr, "%s\n", TTF_GetError());
		exit(1);
	}

	char *sfxpath = "C:\\Users\\chiha\\Desktop\\moiquicodedesfonctions\\editor\\assets\\sound\\";
	char file[1024];
	int j = 0;
	for (int i = 1; i < 8; i++ )
	{
		sprintf(file, "%s%d%s", sfxpath, i, "pressed.wav");
		printf("file: %s\n", file);
		sfx_key[j].pressed = Mix_LoadWAV(file);
		if (sfx_key[j].pressed == NULL)
		{
			fprintf(stderr, "Error loading: %s\n", file);
			exit(1);
		}

		sprintf(file, "%s%d%s", sfxpath, i, "released.wav");
		printf("file: %s\n", file);
		sfx_key[j].released = Mix_LoadWAV(file);
		sfx_key[j].index = j;
		sfx_key[j].is_pressed = false;
		if (sfx_key[j].released == NULL)
		{
			fprintf(stderr, "Error loading: %s\n", file);
			exit(1);
		}
		j++;
	}
	opp = Mix_LoadMUS("C:\\Users\\chiha\\Music\\Oppenheimer (Original Motion Picture Soundtrack) - Ludwig Goransson (2023) [FLAC]\\1.02. Can You Hear The Music.flac");
	if (!opp)
	{
		fprintf(stderr, "Error loading opp\n");
		exit(1);
	}
}

int
/* WinMain() */
main()
{
	volume = 20;
	volume1 = 20;
	srand(time(NULL));
	sfx_index = -1;
	buff_end = 0;
	char *font_path = "C:\\Users\\chiha\\Desktop\\moiquicodedesfonctions"
		"\\editor\\assets\\output.png";
	char *font_path2 = "C:\\Users\\chiha\\Desktop\\moiquicodedesfonctions"
		"\\editor\\assets\\5x6-10x.png";

	init();
	font1 = font_load_from_file(inst.renderer, font_path);
	font2 = font_load_from_file(inst.renderer, font_path2);
	SDL_Texture *txtTexture = 0;
	SDL_Color color = {255, 255, 255, 255};
	Vec2f pos = vec2f(0, 100);
	memset(buff_txt, 0, BUFF_MAX);

	while (running)
	{
		SDL_SetRenderDrawColor(inst.renderer, 50, 50, 50, 255);
		SDL_RenderClear(inst.renderer);
		Events(inst.e);
		/* txtTexture = createTextureForTxt(buff_txt, color); */
		render_text_sized(inst.renderer, &font1, buff_txt, buff_end,
				pos, color, FONT_SCALE);
        
		render_cursor(inst.renderer, color);
		SDL_RenderPresent(inst.renderer);
		SDL_DestroyTexture(txtTexture);
		txtTexture = 0;
	}
	for (int i = 0; i < 7; i++)
	{
		Mix_FreeChunk(sfx_key[i].pressed);
		Mix_FreeChunk(sfx_key[i].released);
	}
	Mix_FreeMusic(opp);
	TTF_CloseFont(font);
	SDL_DestroyRenderer(inst.renderer);
	SDL_DestroyWindow(inst.window);
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	return 0;
}
