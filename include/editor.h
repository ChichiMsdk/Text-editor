#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "vector.h"

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0
#define BUFF_MAX 1024
#define LINE_SIZE_MAX 1024

#define FONT_WIDTH2 1120
#define FONT_HEIGHT2 480
#define FONT_COLS2 16
#define FONT_ROWS2 6
#define FONT_CHAR_WIDTH2  (FONT_WIDTH2  / FONT_COLS2)
#define FONT_CHAR_HEIGHT2 (FONT_HEIGHT2 / FONT_ROWS2)
#define FONT_SCALE2 2
#define ASCII_DISPLAY_LOW2 32
#define ASCII_DISPLAY_HIGH2 126

/*
 * #define FONT_WIDTH 128
 * #define FONT_HEIGHT 64
 */
#define FONT_WIDTH 640
#define FONT_HEIGHT 320
#define FONT_COLS 18
#define FONT_ROWS 7
#define FONT_CHAR_WIDTH  (FONT_WIDTH  / FONT_COLS)
#define FONT_CHAR_HEIGHT (FONT_HEIGHT / FONT_ROWS)
#define FONT_SCALE 2
#define ASCII_DISPLAY_LOW 32
#define ASCII_DISPLAY_HIGH 126

typedef struct sfx
{
	Mix_Chunk	*pressed;
	Mix_Chunk 	*released;
	int			index;
	bool		is_pressed;
}sfx;

typedef struct Font
{
	SDL_Texture *spritesheet;
	SDL_FRect glyph_table[ASCII_DISPLAY_HIGH - ASCII_DISPLAY_LOW + 1];
}Font;

// Check padding
typedef struct YUinstance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect rect;
	SDL_Event e;
}YUinstance;

typedef struct Text
{
	SDL_Texture *texture;
	TTF_Font	*font;
}Text;

int							buff_end;
static unsigned int			g_nl;
static YUinstance			inst;
extern char					buff_txt[BUFF_MAX];
extern float				textWidth;
extern float				textHeight;
extern int					WINDOW_WIDTH;
extern int					WINDOW_HEIGHT;
extern int					running;
extern TTF_Font				*font;
extern Font					font2;
extern sfx					sfx_key[7];
extern Mix_Music			*opp;
extern int					sfx_index;
extern int					volume;
extern int					volume1;
extern Mix_Chunk			*sfx_autre;

// editor.c
int				isText(Uint32 key);
void			isDeleting(void);
void			isTyping(Uint32 event);
void			Events(SDL_Event e);
size_t			yu_print(const char *msg);
SDL_Surface*	SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, 
		int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

// render.c
void			render_text_sized(SDL_Renderer *renderer, Font *font, 
		const char *text, size_t text_size, Vec2f pos, SDL_Color color, float scale);

Font			font_load_from_file(SDL_Renderer *renderer, const char *file_path);

void			render_cursor(SDL_Renderer *renderer, SDL_Color color);

// error.c
				// window renderer surface font
void			logger(void *w, void *r, void *s, void *f, const char *msg);
