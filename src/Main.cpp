/**
 * Tower Defence Pro #005
 * (C) 2010 - Samuel Andersson, Björn Ekberg
 */

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Core/Game.h>
#include <Core/GameEngine.h>
#include <ctime>
#include <memory>
#include <Utils/Log.h>

int main(int argc, char *argv[])
{
	/** Seed srand() */
	time_t t;
	time(&t);
	srand((unsigned int) t);

	Log::reportingLevel() = logDEBUG;
	FILE* logFd;
	fopen_s(&logFd, "output.log", "w");
	Output2FILE::stream() = logFd;

	int ret = 0;
	Game* game = new Game();
	{
		GameSettings settings;
		settings.WindowWidth = WWIDTH;
		settings.WindowHeight = WHEIGHT;
		settings.WindowTitle = "Tower Defence Pro " + std::string(VERSION) + " © 2010 - 2016 A15 Entertainment";

		GameEngine engine(settings);
		ret = engine.run(game);
	}
	delete game;


#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
	return ret;
}
