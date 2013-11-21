

game.pi: game.c
	@echo "\nBuilding for Raspberry PI - if on OSX, use 'make osx' instead\n"
	cc -g -o game.pi game.c lib/ccss/ccss.o -L/usr/lib/arm-linux-gnueabihf -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lSDLmain
osx: game.osx

game.osx: game.c
	@echo "Building for OSX"
	cc -g -o game.osx -I lib/inc game.c lib/ccss/ccss.o -L/opt/local/lib -lSDL -lSDL_image -lSDL_ttf -lSDLmain lib/libs/libSDL_gfx.a -Wl,-framework,AppKit
