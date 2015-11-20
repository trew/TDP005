/**
 * Tower Defence Pro #005
 * (C) 2010 - Samuel Andersson, Björn Ekberg
 */

#include <Core/Game.h>
#include <ctime>

/** Written in App.cpp */

int main(int argc, char *argv[]) {
	/** Seed srand() */
	time_t t;
	time(&t);
	srand(t);

	Game* new_game = new Game();

	return new_game->on_execute();
}
