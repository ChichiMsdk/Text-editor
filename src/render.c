#include "editor.h"

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

SDL_Surface *surface_from_file(const char *file_path)
{
    int width, height, n;
    unsigned char *pixels = 
		stbi_load(file_path, &width, &height, &n, STBI_rgb_alpha);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 rmask = 0xff000000;
    const Uint32 gmask = 0x00ff0000;
    const Uint32 bmask = 0x0000ff00;
    const Uint32 amask = 0x000000ff;
#else // little endian, like x86
    const Uint32 rmask = 0x000000ff;
    const Uint32 gmask = 0x0000ff00;
    const Uint32 bmask = 0x00ff0000;
    const Uint32 amask = 0xff000000;
#endif

    const int depth = 32;
    const int pitch = 4*width;

    return SDL_CreateRGBSurfaceFrom( (void*)pixels, width, height, depth, pitch,
                   rmask, gmask, bmask, amask);
}

#define ASCII_DISPLAY_LOW 32
#define ASCII_DISPLAY_HIGH 126

Font
font_load_from_file(SDL_Renderer *renderer, const char *file_path)
{
    Font font = {0};

    SDL_Surface *font_surface = surface_from_file(file_path);
    font.spritesheet = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_DestroySurface(font_surface);

    for (size_t ascii = ASCII_DISPLAY_LOW; ascii <= ASCII_DISPLAY_HIGH; ++ascii)
	{
        const size_t index = ascii - ASCII_DISPLAY_LOW;
        const size_t col = index % FONT_COLS;
        const size_t row = index / FONT_COLS;
        font.glyph_table[index] = (SDL_FRect) {
            .x = col * FONT_CHAR_WIDTH,
            .y = row * FONT_CHAR_HEIGHT,
            .w = FONT_CHAR_WIDTH,
            .h = FONT_CHAR_HEIGHT,
        };
    }
    return font;
}

void 
render_char(SDL_Renderer *renderer, Font *font, char c, Vec2f pos,
		float scale)
{
    const SDL_FRect dst =
	{
        .x = floorf(pos.x),
        .y = floorf(pos.y),
        .w = floorf(FONT_CHAR_WIDTH * scale),
        .h = floorf(FONT_CHAR_HEIGHT * scale),
    };

    assert(c >= ASCII_DISPLAY_LOW);
    assert(c <= ASCII_DISPLAY_HIGH);
    const size_t index = c - ASCII_DISPLAY_LOW;
    SDL_RenderTexture(
			renderer, font->spritesheet, &font->glyph_table[index], &dst);
}

void 
render_text_sized(SDL_Renderer *renderer, Font *font, const char *text,
		size_t text_size, Vec2f pos, SDL_Color color, float scale)
{
    SDL_SetTextureColorMod(
            font->spritesheet,
            color.r,
            color.g,
            color.b);

    SDL_SetTextureAlphaMod(font->spritesheet, color.a);

    Vec2f pen = pos;
    for (size_t i = 0; i < text_size; ++i)
	{
		if (pen.x >= WINDOW_WIDTH)
			break;
        render_char(renderer, font, text[i], pen, scale);
        pen.x += FONT_CHAR_WIDTH * scale;
    }
}

void 
render_text(SDL_Renderer *renderer, Font *font, const char *text,
		Vec2f pos, SDL_Color color, float scale)
{
    render_text_sized(renderer, font, text, strlen(text), pos, color, scale);
}

void 
render_cursor(SDL_Renderer *renderer, SDL_Color color)
{
    const SDL_FRect rect = {
        .x = (int) floorf(buff_end * FONT_CHAR_WIDTH * FONT_SCALE),
        .y = 100,
        .w = FONT_CHAR_WIDTH, 
        .h = FONT_CHAR_HEIGHT * FONT_SCALE,
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}


/*
 * void
 * render_text(SDL_Renderer *renderer, SDL_Texture *texture, float scale, 
 * 		char *msg, SDL_Color color, Vec2f pos)
 * {
 * 	if (texture == 0)
 * 		return ;
 * 	SDL_Surface *textSurface =
 * 		TTF_RenderUTF8_Shaded(font, msg, color,(SDL_Color){255, 255, 255, 255});
 * 	if (textSurface == NULL)
 * 	{
 * 		logger(inst.window, inst.renderer, textSurface, font, TTF_GetError());
 * 	}
 * 	textWidth = textSurface->w;
 * 	textHeight = textSurface->h;
 * 	SDL_Texture *textTexture = 
 * 		SDL_CreateTextureFromSurface(inst.renderer, textSurface);
 * 	if (textTexture == NULL)
 * 	{
 * 		logger(inst.window, inst.renderer, textSurface, font, SDL_GetError());
 * 	}
 * 	SDL_DestroySurface(textSurface);
 * 	SDL_FRect renderQuad = { 50.0f, 50.0f, textWidth, textHeight };
 * 	SDL_RenderTexture(inst.renderer, texture, NULL, &renderQuad);
 * }
 */

/*
 * SDL_Texture*
 * createTextureForTxt(char *text, SDL_Color textColor)
 * {
 * 	if (buff_end <= 0)
 * 	{
 * 		return NULL;
 * 	}
 * 	SDL_Surface *textSurface =
 * 		TTF_RenderUTF8_Shaded(font, text, textColor,(SDL_Color){0, 0, 0, 255});
 *     if (textSurface == NULL)
 * 	{
 * 		logger(inst.window, inst.renderer, textSurface, font, TTF_GetError());
 *     }
 * 	textWidth = textSurface->w;
 * 	textHeight = textSurface->h;
 *     SDL_Texture *textTexture = 
 * 		SDL_CreateTextureFromSurface(inst.renderer, textSurface);
 *     if (textTexture == NULL)
 * 	{
 * 		logger(inst.window, inst.renderer, textSurface, font, SDL_GetError());
 *     }
 * 	SDL_DestroySurface(textSurface);
 * 	SDL_FRect renderQuad = { 50.0f, 50.0f, textWidth, textHeight };
 * 	SDL_RenderTexture(inst.renderer, textTexture, NULL, &renderQuad);
 * 	return textTexture;
 * }
 */
