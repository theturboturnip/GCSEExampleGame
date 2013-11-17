
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "./lib/ccss/ccss.h"

#define SCREEN_WIDTH 		360
#define SCREEN_HEIGHT 		360
#define SCREEN_BPP 		32

#define FRAMES_PER_SECOND	24

#define BOOL 			unsigned char
#define TRUE 			1
#define FALSE 			0

#define WHITE		{255, 255, 255}
#define moveSpeed               3

typedef  struct{
	int x,y,w,h;
}collision_box;

collision_box *setUpWalls( void )
{
    static const char filename[]="walls.txt";
    int x=1,y=-1,wall_number=0;
    collision_box *walls;
    collision_box *wall_box;
    wall_box=malloc(sizeof(collision_box));
    walls=malloc(sizeof(collision_box)*10);
    wall_box->w=60;
    wall_box->h=60;
    //char rooms[maxX][maxY];
    FILE *file = fopen ( filename, "rt" );
    if ( file != NULL )
    {
        char line [ 128 ]; /* or other suitable maximum line size */
        while ( (fgets ( line, sizeof(line), file ) != NULL) && (wall_number<10)) /* read a line */
        {
            if(x==1)
            {
                sscanf(line,"%d",&(wall_box->x)); //=(int)line;
            }else if(y==1)
            {
                sscanf(line,"%d",&(wall_box->y)); //=(int)line;
                // wall_box->y=(int)line;
                walls[wall_number]=*wall_box;
                walls[wall_number].w=60;
                walls[wall_number].h=60;
                //printf("Created wall %d with x,y (%d,%d)\n",wall_number,walls[wall_number].x,walls[wall_number].y);
                wall_number++;
            }
            x=-x;
            y=-y;
        }
        fclose ( file );
    }
    else
    {
        perror ( filename ); /* why didn't the file open? */
    }
    return walls;
}
int collide(collision_box box1,collision_box box2){
	int currentNum=box1.x;
	while (currentNum>box2.x+box2.w||currentNum<box2.x){
		if(currentNum>box1.x+box1.w){
			return 0;
		}
		currentNum++;
	}
	currentNum=box1.y;
	while (currentNum>box2.y+box2.h||currentNum<box2.y){
		if(currentNum>box1.y+box1.h){
			return 0;
		}
		currentNum++;
	}
	return 1;
}
extern int main(int argc,char *argv[] ){
	BOOL won = FALSE;
	BOOL quit = FALSE;
	// surfaces
	SDL_Surface* screen = NULL;
	SDL_Surface* background = NULL;
	
	SDL_Surface* character = NULL;
	SDL_Surface* wallImg = NULL;
	SDL_Surface* endFlag = NULL;

	SDL_Surface* up = NULL;
	SDL_Surface* upLeft = NULL;
	SDL_Surface* left = NULL;
	SDL_Surface* downLeft = NULL;
	SDL_Surface* down = NULL;
	SDL_Surface* downRight = NULL;
	SDL_Surface* right = NULL;
	SDL_Surface* upRight = NULL;

	SDL_Event event;
	// font
	TTF_Font* font = NULL;
	SDL_Color text_color = WHITE;
	// game data
	int x=0,y=0, numOfTicks=0, highestTicks=10000 , score;
	unsigned char *keystates;
	int starttick, ticks;

	collision_box Pbox;
	Pbox.h=30;
	Pbox.w=30;
	collision_box emptyBox;
	emptyBox.w=60;
	emptyBox.h=60;
	collision_box *walls;
	collision_box endBox;
	endBox.x=300;
	endBox.y=300;
	endBox.w=60;
	endBox.h=60;
	int i;
	/*for (i=0;i<10;i++){
		walls[i]=emptyBox;
	}
	walls[0].x=60;
	walls[0].y=0;
	walls[1].x=120;
	walls[1].y=0;
	walls[2].x=120;
	walls[2].y=60;
	walls[3].x=240;
	walls[3].y=60;
	walls[4].x=60;
	walls[4].y=120;
	walls[5].x=240;
	walls[5].y=180;
	walls[6].x=300;
	walls[6].y=180;
	walls[7].x=60;
	walls[7].y=240;
	walls[8].x=120;
	walls[8].y=240;
	walls[9].x=300;
	walls[9].y=240;
    */
    walls=setUpWalls();
	printf("Game starting...\n");

	screen = CCSS_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, "Our maze game!");
	if(screen==NULL){
		printf("Something went wrong!\n");
		exit(-1);
	}

	// Load background image
	background = CCSS_load_image("./resources/img/tux.png");
	// load character and resize it
	wallImg = CCSS_load_and_resize_image("./resources/img/smiley.png", 0.1, 0.1);

	up = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 180);
	upLeft = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 225);
	left = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 270);
	downLeft = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 315);
	down = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 0);
	downRight = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 45);
	right = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 90);
	upRight = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 135);

	endFlag = CCSS_load_and_resize_image("./resources/img/smiley.png", 0.075,0.075);
	// load font
	font = TTF_OpenFont( "./resources/font/creaminal.ttf", 14 );	

	while(quit==FALSE){
		starttick = SDL_GetTicks();
		while(SDL_PollEvent( &event )){ // while there is event to handle
			if(event.type == SDL_QUIT){
				quit = TRUE;
			}
		}
		keystates = SDL_GetKeyState( NULL );
        if(keystates[SDLK_LEFT]){
			character=left;
			x-=moveSpeed;
		}else if(keystates[SDLK_RIGHT]){
			character=right;
			x+=moveSpeed;
		}
		if(keystates[SDLK_UP]){
			y-=moveSpeed;
			character=up;
			if(keystates[SDLK_LEFT])
            {
				character=upLeft;
            }
			if(keystates[SDLK_RIGHT])
            {
				character=upRight;
            }
        }
		else if(keystates[SDLK_DOWN]){
			y+=moveSpeed;
			character=down;
			if(keystates[SDLK_LEFT])
            {
				character=downLeft;
            }
			if(keystates[SDLK_RIGHT])
            {
				character=downRight;
            }
		}

		if(x<0)
			x=0;
		else if(x+Pbox.w>SCREEN_WIDTH)
			x=SCREEN_WIDTH-Pbox.w;
		if(y<0)
			y=0;
		else if(y+Pbox.h>SCREEN_HEIGHT)
			y=SCREEN_HEIGHT-Pbox.h;
	
		/*if (x>=wall1.x&&x<=wall1.x+60&&y==wall1.y+60)
		{
			//Character is below Wall
			y++;
		}*/

		Pbox.x=x;
		Pbox.y=y; 
		for (i=0;i<10;i++){
			if (collide(Pbox, walls[i])==1){
				x=0;
				y=0;
			}
		}
		// Apply background to screen
		CCSS_apply_surface(0, 0, background, screen);		
		// Apply our character
		CCSS_apply_surface(x, y, character, screen);
		CCSS_apply_surface(endBox.x,endBox.y,endFlag,screen);
		// We print something
		CCSS_print(400, 0, font, text_color, screen, "Position %d-%d", x, y); 
		for (i=0;i<10;i++){
			CCSS_apply_surface(walls[i].x, walls[i].y, wallImg, screen);
		}
		CCSS_print(100,50, font, text_color, screen, "Score: %d", numOfTicks);
		// Update 
		SDL_Flip( screen );
		ticks = SDL_GetTicks() - starttick;
		if(ticks < 1000 / FRAMES_PER_SECOND){
			SDL_Delay((1000/FRAMES_PER_SECOND)-ticks);
		}
		if (collide(Pbox, endBox)==1){
			CCSS_apply_surface(0, 0, background, screen);
			CCSS_print(100, 0, font, text_color, screen, "YOU WIN!!!");
			score=highestTicks-numOfTicks;
			CCSS_print(100,50, font, text_color, screen, "Score: %d", score);
			won=TRUE;
			quit=TRUE;
		}
		numOfTicks+=9;
	}
	if(won==TRUE){
		SDL_Flip( screen );
		SDL_Delay(3000);
	}
	printf("Closing the game...\n");
	// Free background
	SDL_FreeSurface( background );
	SDL_FreeSurface( character );
	// unload font
	TTF_CloseFont(font);
	
	// Quite SDL
	SDL_Quit();

	
	return 0;
}
