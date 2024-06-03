#include "editor.h"
#include "SDL3/SDL_keycode.h"
#include <io.h>

int
isText(Uint32 key)
{
	if (key < 127 && key >= 32)
		return TRUE;
	return FALSE;
}

// NOTE: make it based on the keypressed
void
play_sfx(int state)
{
	/* int channel = (rand() % 7) + 1; */
	int channel = -1;
	if (sfx_index != -1)
	{
		if (state == 1)
		{
			if (sfx_key[sfx_index].is_pressed == true)
				return;
			if (Mix_PlayChannel(channel, sfx_key[sfx_index].pressed, 0) == -1)
				printf("no free channel: sfx_index = %d, %d\n", sfx_index, channel);
			sfx_key[sfx_index].is_pressed = true;
			printf("pressing\n");
			return;
		} 
		else
		{
			if (Mix_PlayChannel(channel, sfx_key[sfx_index].released, 0) == -1)
				printf("no free channel: sfx_index = %d, %d\n", sfx_index, channel);
			sfx_key[sfx_index].is_pressed = false;
			sfx_index = (rand() % 7); 
			printf("releasing\n");
			return;
		}
	}
	// this happens at the first sfx played in since program's launch
	sfx_index = (rand() % 7); 
	printf("sfx_index: %d\n", sfx_index);
	Mix_Volume(-1, volume);
	if (Mix_PlayChannel(channel, sfx_key[sfx_index].pressed, 0) == -1)
		printf("Sound didn't play\n");
	sfx_key[sfx_index].is_pressed = true;
	printf("sfx_index was -1 somehow (should happen only once) channel: %d\n", channel);
}

void
play_music()
{
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(opp, -1);
		Mix_VolumeMusic(volume1);
		printf("Music started\n");
		return;
	}
	if (Mix_PausedMusic() == 0)
	{
		Mix_PauseMusic();
		printf("music paused\n");
	}
	else
	{
		Mix_ResumeMusic();
		printf("music resumed\n");
	}
}

void
Events(SDL_Event e)
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			running = 0;
		}
		else if (e.type == SDL_EVENT_KEY_DOWN)
		{
			play_sfx(1);
			switch (e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					running = 0;
					break;
				case SDLK_BACKSPACE:
					isDeleting();
					break;
				case SDLK_r:
					play_music();
					break;
				case SDLK_UP:
					volume++;
					if (volume >= MIX_MAX_VOLUME)
						volume = MIX_MAX_VOLUME;
					Mix_Volume(-1, volume);
					printf("volume: %d\n", volume);
					break;
				case SDLK_DOWN:
					volume--;
					if (volume < 0)
						volume = 0;
					Mix_Volume(-1, volume);
					printf("volume: %d\n", volume);
					break;
				case SDLK_RIGHT:
					volume1++;
					if (volume1 > MIX_MAX_VOLUME)
						volume1 = MIX_MAX_VOLUME;
					Mix_VolumeMusic(volume1);
					printf("volume: %d\n", volume1);
					break;
				case SDLK_LEFT:
					volume1--;
					if (volume1 < 0)
						volume1 = 0;
					Mix_VolumeMusic(volume1);
					printf("volume: %d\n", volume1);
					break;
				default:
					isTyping(e.key.keysym.sym);
					break;
			}
		}
		else if (e.type == SDL_EVENT_KEY_UP)
		{
			play_sfx(0);
		}
	}
}

/** 
 * NOTE: each char * is printed until '\n' then increment etc.. whithin loop
 * maybe better way than go character by character .. 
 */

size_t
yu_print(const char *msg)
{
	int count = 0;
	char temp;
	while (*msg != 0)
	{
		temp = count + '0';
		_write(1, &temp, 1);
		_write(1, "\t", 1);
		while (*msg != 0)
		{
			if (*msg == '\n')
			{
				_write(1, msg, 1);
				msg++;
				break;
			}
			_write(1, msg, 1);
			msg++;
		}
		count++;
	}
	return count;
}

void 
clearLine()
{
    printf("\r");
	// Clear the line by overwriting with spaces
	printf("                                                                  ");
    printf("\r");
    fflush(stdout);
}

void 
isTyping(Uint32 event_key)
{
	static Uint32 key;
	key = event_key;
	if (key == SDLK_RETURN)
	{
		buff_txt[buff_end] = '\n';
		buff_end++;
		g_nl++;
		/* clearLine(); */
		/* yu_print(buff_txt); */
		assert(buff_end < LINE_SIZE_MAX);
		return ;
	}
	if (isText(key) == FALSE)
	{
		return ;
	}
	buff_txt[buff_end] = key;
	buff_end++;
	/* clearLine(); */
	/* yu_print(buff_txt); */
	assert(buff_end < LINE_SIZE_MAX);
}

void
isDeleting(void)
{
	if (buff_end > 0)
	{
		// deletes last character entry
		buff_end--;
		if (buff_txt[buff_end] == '\n')
		{
			g_nl--;
		}
		buff_txt[buff_end] = 0;
	}
	/* clearLine(); */
	/* yu_print(buff_txt); */
}
