/**
 * Tower Defence Pro #005
 * (C) 2010 - Samuel Andersson, Björn Ekberg
 */

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Core/Game.h>
#include <ctime>

/** Written in App.cpp */

int main(int argc, char *argv[]) {
	/** Seed srand() */
	time_t t;
	time(&t);
	srand(t);

	Game* new_game = new Game();

	int ret = new_game->on_execute();
	delete new_game;
	new_game = NULL;

	_CrtDumpMemoryLeaks();
	return ret;
}
