#include "ccss.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <stdarg.h>

void CCSS_print(int x, int y, TTF_Font* font,  SDL_Color text_color, SDL_Surface* screen, const char* fmt, ...){
	SDL_Surface* message = NULL;
	char tmp[2048];
	va_list args;

	va_start(args, fmt);
	vsprintf(tmp, fmt, args);
	va_end(args);
	message = TTF_RenderText_Solid(font, tmp, text_color);
	if(message!=NULL){
		CCSS_apply_surface(x, y, message, screen);
	}
	SDL_FreeSurface(message);
}

SDL_Surface* CCSS_init(int screen_width, int screen_height, int screen_bpp, const char* caption){
	SDL_Surface* screen = NULL;

	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1)
		return NULL;
	screen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_SWSURFACE);
	if(screen==NULL)
		return NULL;
	SDL_WM_SetCaption(caption, NULL);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	TTF_Init();
	return screen; // everything worked!	
}

void CCSS_apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination){
	SDL_Rect offset;
	offset.x=x;
	offset.y=y;
	SDL_BlitSurface(source, NULL, destination, &offset);
}

SDL_Surface* CCSS_load_image(const char* filename){
	// Temporary surface pointer
	SDL_Surface* tmp = NULL;
	// Optimized image
	SDL_Surface* opti = NULL;

	tmp = IMG_Load(filename);
	if(tmp!=NULL){
		opti = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);
	}
	return opti;
}

SDL_Surface* CCSS_load_and_resize_image(const char* filename, double width, double height){
	// Temporary surface pointer
	SDL_Surface* tmp = NULL;
	// Optimized image
	SDL_Surface* opti = NULL;

	tmp = IMG_Load(filename);
	if(tmp!=NULL){
		tmp = zoomSurface(tmp, width, height, 1);
		opti = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);
	}
	return opti;
}

SDL_Surface* CCSS_load_resize_and_rotate(const char* filename, double zoom, double angle){
	// Temporary surface pointer
	SDL_Surface* tmp = NULL;
	// Optimized image
	SDL_Surface* opti = NULL;
	
	tmp = IMG_Load(filename);
	if(tmp!=NULL){
		tmp = rotozoomSurface(tmp, angle, zoom, 1);
		opti = SDL_DisplayFormatAlpha(tmp);		
		SDL_FreeSurface(tmp);
	}
	return opti;
}


