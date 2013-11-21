game.pi: game.c
	cc -g -o game.pi game.c lib/ccss/ccss.o -L/usr/lib/arm-linux-gnueabihf -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lSDLmain

game.osx: game.c
	cc -g -o game.osx -I lib/inc game.c lib/ccss/ccss.o -L/opt/local/lib -lSDL -lSDL_image -lSDL_ttf -lSDLmain lib/libs/libSDL_gfx.a -Wl,-framework,AppKit
