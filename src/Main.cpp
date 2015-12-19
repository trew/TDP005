/**
 * Tower Defence Pro #005
 * (C) 2010 - Samuel Andersson, Björn Ekberg
 */

#if defined(_WIN32) && (_DEBUG)
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

#ifdef _DEBUG
	Log::reportingLevel() = logDEBUG;
#else
	Log::reportingLevel() = logWARNING;
#endif
	FILE* logFd;
	fopen_s(&logFd, "log.txt", "w");
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


#if defined(_WIN32) && defined (_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
	return ret;
}
