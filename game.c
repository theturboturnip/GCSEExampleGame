
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "./lib/ccss/ccss.h"

#define SCREEN_WIDTH 		580
#define SCREEN_HEIGHT 		540
#define SCREEN_BPP 		32

#define FRAMES_PER_SECOND	24

#define BOOL 			unsigned char
#define TRUE 			1
#define FALSE 			0

#define WHITE		{255, 255, 255}
#define moveSpeed               3

#define NUM_OF_WALLS 39
typedef  struct{
	int x,y,w,h;
}collision_box;

int collide(collision_box box1,collision_box box2){
	int currentNum=box1.x;
	while (currentNum>=box2.x+box2.w||currentNum<=box2.x){
		if(currentNum>=box1.x+box1.w){
			return 0;
		}
		currentNum++;
	}
	currentNum=box1.y;
	while (currentNum>=box2.y+box2.h||currentNum<=box2.y){
		if(currentNum>=box1.y+box1.h){
			return 0;
		}
		currentNum++;
	}
	return 1;
}
int collideFromPointer(collision_box box1, collision_box *box2){
    collision_box box3;
    box3.x=box2->x;
    box3.y=box2->y;
    box3.w=box2->w;
    box3.h=box2->h;
    return collide(box1,box3);
}
int randomNumber(){
    //sleep(1);
    return (rand()%5)*60;
}
collision_box *setUpItem( collision_box *walls )
{
    collision_box *item;
    item=malloc(sizeof(collision_box));
    item->w=60;
    item->h=60;
    int i;
    while(1){
        item->x=randomNumber();
        item->y=randomNumber();
        for(i=0;i<10;i++){
            if(!collideFromPointer(walls[i],item)){
                return item;
            }
        }
    }
}
collision_box *setUpWalls( void )
{
    static const char filename[]="walls.txt";
    int x=1,y=-1,wall_number=0,i=0;
    collision_box *walls;
    collision_box *wall_box;
    wall_box=malloc(sizeof(collision_box));
    walls=malloc(sizeof(collision_box)*NUM_OF_WALLS);
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
    for (i=0;i<8;i++){
        wall_box->x=360;
        wall_box->y=(i-1)*60;
        walls[i+10]=*wall_box;
        
    }
    for (i=0;i<8;i++){
        wall_box->y=360;
        wall_box->x=(i-1)*60;
        walls[i+17]=*wall_box;        
    }
    for (i=0;i<8;i++){
        wall_box->x=-60;
        wall_box->y=(i-1)*60;
        walls[i+24]=*wall_box;        
    }
    for (i=0;i<8;i++){
        wall_box->y=-60;
        wall_box->x=(i-1)*60;
        walls[i+31]=*wall_box;        
    }
    return walls;
}
int *getScores(void){
    int i=0,score;
    int *scores=malloc(sizeof(int)*5);
    FILE *file = fopen ( "scores.txt", "r" );
    for (i=0; i<5; i++) scores[i]=0;
    i=0;
    if ( file != NULL )
    {
        char line [ 128 ]; /* or other suitable maximum line size */
        while ( (fgets ( line, sizeof(line), file ) != NULL) && (i<5)) /* read a line */
        {
            score=atoi(line);
            //printf("Score;%d\n",score);
            scores[i]=score;
            i++;    
        }
        fclose ( file );
    }
    else
    {
        perror ( "scores.txt" ); /* why didn't the file open? */
    }
    return scores;
}
void setScores(int score){
    int *scores=getScores();
    int i,o;
    FILE *fp;
    fp = fopen( "scores.txt" , "w" );
    
    if(fp!=NULL){
        for (i=0;i<5;i++){
            if(score>scores[i]){
                for(o=4-i;o>0;o--){
                    scores[o]=scores[o-1];
                }
                scores[i]=score;
                for(i=0;i<5;i++){
                    fprintf(fp,"%d\n",scores[i]);
                }
                break;
            }
        }
    }else{
        return;
    }
    fclose(fp);
}
    
extern int main(int argc,char *argv[] ){
    srand(time(NULL));

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

	SDL_Surface* itemImg = NULL;

	SDL_Event event;
	// font
	TTF_Font* font = NULL;
    TTF_Font* logoFont=NULL;
	SDL_Color text_color = WHITE;
	// game data
	int x=15,y=15, numOfTicks=0, highestTicks=50000 , score,time,points=0,i,spawnX=x,spawnY=y;
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
    collision_box *points_box;
    points_box=setUpItem(walls);
	printf("Game starting...\n");   
	screen = CCSS_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, "MAZE: The Game");
	if(screen==NULL){
		printf("Something went wrong!\n");
		exit(-1);
	}

	// Load background image
	background = CCSS_load_image("./resources/img/tux.png");
	// load character and resize it
	itemImg = CCSS_load_and_resize_image("./resources/img/cherry.png", 1, 1);

	wallImg = CCSS_load_and_resize_image("./resources/img/wall.png", 0.2, 0.2);

	up = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 180);
	upLeft = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 225);
	left = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 270);
	downLeft = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 315);
	down = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 0);
	downRight = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 45);
	right = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 90);
	upRight = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 135);

	endFlag = CCSS_load_and_resize_image("./resources/img/flag.png", 0.1,0.1);
	// load font
	font = TTF_OpenFont( "./resources/font/creaminal.ttf", 14 );	
    logoFont = TTF_OpenFont( "./resources/font/creaminal.ttf", 30 );
	while(quit==FALSE){
        character=down;
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

		/*if(x<0)
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
		for (i=0;i<NUM_OF_WALLS;i++){
			if (collide(Pbox, walls[i])==1){
				x=spawnX;
				y=spawnY;
			}
            if (collideFromPointer(walls[i],points_box)==1){
			    points_box=setUpItem(walls);
			}
		}
        if(collideFromPointer(Pbox,points_box)==1){
            points+=500;
            points_box=setUpItem(walls);
		}
		// Apply background to screen
		CCSS_apply_surface(0, 0, background, screen);		
		// Apply our character
		CCSS_apply_surface(x, y, character, screen);
		CCSS_apply_surface(endBox.x,endBox.y,endFlag,screen);
		CCSS_apply_surface(points_box->x,points_box->y,itemImg,screen);
		// We print something
		for (i=0;i<NUM_OF_WALLS;i++){
			CCSS_apply_surface(walls[i].x, walls[i].y, wallImg, screen);
		}
		CCSS_print(440,0, logoFont, text_color, screen, "MAZE");
		CCSS_print(445,30, font, text_color, screen, "The Game");
		CCSS_print(435,80, font, text_color, screen, "Score:");
		CCSS_print(450,100, font, text_color, screen, "%d",points);

		CCSS_print(435,120, font, text_color, screen, "Time bonus:");
		CCSS_print(465,140, font, text_color, screen, "%d",highestTicks-numOfTicks);
		// Update 
		SDL_Flip( screen );
		ticks = SDL_GetTicks() - starttick;
		if(ticks < 1000 / FRAMES_PER_SECOND){
			SDL_Delay((1000/FRAMES_PER_SECOND)-ticks);
		}
		if (collide(Pbox, endBox)==1){
			CCSS_apply_surface(0, 0, background, screen);
			CCSS_print(100, 0, font, text_color, screen, "YOU WIN!!!");
			time=highestTicks-numOfTicks;
            score=points+time;
			CCSS_print(100,50, font, text_color, screen, "Points: %d", points);
			CCSS_print(50,100, font, text_color, screen, "Time Bonus: %d", time);
			CCSS_print(25,200, font, text_color, screen, "Total Score: %d", score);
			won=TRUE;
			quit=TRUE;
		}
		numOfTicks+=9;
	}
	if(won==TRUE){
		SDL_Flip( screen );
		SDL_Delay(3000);
		CCSS_apply_surface(0, 0, background, screen);
        setScores(score);
        int *scores=getScores();
        //printf("1:%d\n2:%d\n3:%d\n4:%d\n5:%d\n",scores[0],scores[1],scores[2],scores[3],scores[4]);
        CCSS_print(100,0, font,text_color,screen,"Your score: %d",score);
        int scorex=100;
        for(i=0;i<=4;i++){
            CCSS_print(scorex,100+((i-1)*50),font,text_color,screen,"%d:%d",i+1,scores[i]);
        }
        SDL_Flip(screen);
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
