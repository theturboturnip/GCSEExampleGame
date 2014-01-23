
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
                sscanf(line,"%d",&(wall_box->y));
                walls[wall_number]=*wall_box;
                walls[wall_number].w=60;
                walls[wall_number].h=60;
                wall_number++;
            }
            x=-x;
            y=-y;
        }
        fclose ( file );
    }
    else
    {
        perror ( filename ); 
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

    SDL_Surface* playerImgs[360];
	SDL_Surface* itemImg = NULL;

	SDL_Event event;
	// font
	TTF_Font* font = NULL;
    TTF_Font* logoFont=NULL;
	SDL_Color text_color = WHITE;
	// game data
	int x=15,y=15, numOfTicks=0, highestTicks=50000 , score,time,points=0,i,spawnX=x,spawnY=y,angle=0;
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
	background = CCSS_load_image("./resources/img/background.png");
	// load character and resize it
	itemImg = CCSS_load_and_resize_image("./resources/img/cherry.png", 1, 1);

	wallImg = CCSS_load_and_resize_image("./resources/img/wall.png", 0.2, 0.2);

	playerImgs[180] = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 180);
	playerImgs[225] = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 225);
	playerImgs[270] = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 270);
    playerImgs[315] = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 315);
	playerImgs[0]   = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 0);
	playerImgs[45]  = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 45);
	playerImgs[90]  = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 90);
	playerImgs[135] = CCSS_load_resize_and_rotate("./resources/img/smiley.png", 0.05, 135);

	endFlag = CCSS_load_and_resize_image("./resources/img/flag.png", 0.1,0.1);
	// load font
	font = TTF_OpenFont( "./resources/font/creaminal.ttf", 14 );	
    logoFont = TTF_OpenFont( "./resources/font/creaminal.ttf", 30 );
	while(quit==FALSE){
        //1. Get Input
        angle=0;
		starttick = SDL_GetTicks();
		while(SDL_PollEvent( &event )){ // while there is event to handle
			if(event.type == SDL_QUIT){
				quit = TRUE;
			}
		}
		keystates = SDL_GetKeyState( NULL );
        //2+3. Move and rotate character
        if(keystates[SDLK_LEFT]){
            angle+=270;
			x-=moveSpeed;
        }else if(keystates[SDLK_RIGHT]){
            angle+=90;
			x+=moveSpeed;
		}
		if(keystates[SDLK_UP]){
			y-=moveSpeed;
			if (angle>180){
                angle-=45;
            }else if (angle<180&&angle!=0){
                angle+=45;
            }else{
                angle=180;
            }
        }
		else if(keystates[SDLK_DOWN]){
			y+=moveSpeed;
            if (angle<180&&angle!=0){
                angle-=45;
            }else if (angle>180){
                angle+=45;
            }else{
                angle=0;
            }
		}
        if (angle<0){
            angle=0;
        }else if(angle>360){
            angle-=360;
        }
        character=playerImgs[angle];
		Pbox.x=x;
		Pbox.y=y; 
        //4. If colliding with walls, move to start
		for (i=0;i<NUM_OF_WALLS;i++){
			if (collide(Pbox, walls[i])==1){
				x=spawnX;
				y=spawnY;
			}
            if (collideFromPointer(walls[i],points_box)==1){
			    points_box=setUpItem(walls);
			}
		}
        //5. If colliding with cherries, get 500 points & move cherry
        if(collideFromPointer(Pbox,points_box)==1){
            points+=500;
            points_box=setUpItem(walls);
		}
        //6. Draw everything
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
		CCSS_print(465,80, font, text_color, screen, "Score:");
		CCSS_print(470,100, font, text_color, screen, "%d",points);

		CCSS_print(435,120, font, text_color, screen, "Time bonus:");
		CCSS_print(465,140, font, text_color, screen, "%d",highestTicks-numOfTicks);
		// Update 
		SDL_Flip( screen );
		ticks = SDL_GetTicks() - starttick;
		if(ticks < 1000 / FRAMES_PER_SECOND){
			SDL_Delay((1000/FRAMES_PER_SECOND)-ticks);
		}
        //7. If colliding with flags, victory=true
		if (collide(Pbox, endBox)==1){
			CCSS_apply_surface(0, 0, background, screen);
			CCSS_print(225, 0, font, text_color, screen, "YOU WIN!!!");
			time=highestTicks-numOfTicks;
            score=points+time;
			CCSS_print(225,50, font, text_color, screen, "Points: %d", points);
			CCSS_print(200,100, font, text_color, screen, "Time Bonus: %d", time);
			CCSS_print(180,200, font, text_color, screen, "Total Score: %d", score);
			won=TRUE;
			quit=TRUE;
		}
		numOfTicks+=9;
	}
    //8. If victory=true, show scores and then close game
	if(won==TRUE){
		SDL_Flip( screen );
		SDL_Delay(3000);
		CCSS_apply_surface(0, 0, background, screen);
        setScores(score);
        int *scores=getScores();
        CCSS_print(200,0, font,text_color,screen,"Your score: %d",score);
        int scorex=180;
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
