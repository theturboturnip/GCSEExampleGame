#ifndef __CCSS_H
#define __CCSS_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

SDL_Surface* CCSS_init(int screen_width, int screen_height, int screen_bpp, const char* caption);
void CCSS_apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
SDL_Surface* CCSS_load_image(const char* filename);
SDL_Surface* CCSS_load_and_resize_image(const char* filename, double width, double height);
void CCSS_print(int x, int y, TTF_Font* font,  SDL_Color text_color,  SDL_Surface* screen, const char* fmt, ...);
SDL_Surface* CCSS_load_resize_and_rotate(const char* filename, double zoom, double angle);

#endif
