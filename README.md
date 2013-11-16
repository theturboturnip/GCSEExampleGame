ccss_game
=========

SETING THINGS UP
================
sudo apt-get install libsdl-dev

sudo apt-get install libsdl-image1.2-dev

sudo apt-get install libsdl-gfx1.2-dev

sudo apt-get install libsdl-ttf1.2-dev

COMPILING OUR LIBRARY
=====================
cd ./lib/ccss

make Makefile all

cd ../../

COMPILING OUR GAME
==================
gcc game.c ./lib/ccss/ccss.a -o game.o -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

PROXY CONFIGURATION
===================

GIT: git config --global http.proxy http://CCSS\username:password@172.16.1.107:8080

Reset git: git config --global http.proxy ""

Maybe need three back slashes for the domain...


File

/etc/apt/apt.conf.d/80proxy

Acquire::http::proxy "http://<username>:<password>@172.16.1.107:8080";




