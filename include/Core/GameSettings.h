#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <string>

class GameSettings
{
public:
	GameSettings() {}
	virtual ~GameSettings() {}
	
	std::string WindowTitle{ "Game" };
	int WindowWidth{ 640 };
	int WindowHeight{ 480 };
	int MaxFPS { 60 };

	float TimeStep { 1 / 60.f };
	int MaxFrameSkip{ 10 };
};

#endif