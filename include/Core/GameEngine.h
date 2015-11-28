#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <Core/ConfigFile.h>
#include <Core/Game.h>
#include <Core/GameSettings.h>
#include <State/State.h>
#include <Utils/FPS.h>
#include <list>

class State;

class GameEngine
{
public:
	GameEngine(const GameSettings &settings) : settings(settings) {}
	virtual ~GameEngine();

	int run(Game* game);
	void exit();

	State* const popState();
	void pushState(State* const state);
	void setState(State* const state);

	Game* const getGame() const { return game; }
	SDL_Window* const getWindow() const { return window; }
	SDL_Renderer* const getRenderer() const { return renderer; }

	const float getGameSpeed() const { return gameSpeed; }
	void setGameSpeed(const float speed) { gameSpeed = speed; }

	const int getFPS() const { return fps.getFPS(); }

private:
	bool init();
	bool handleEvent(const SDL_Event&);

	GameSettings settings;
	FPS fps;
	float gameSpeed { 1.f };
	ConfigFile* configFile { nullptr };
	Game* game { nullptr };
	SDL_Window* window { nullptr };
	SDL_Renderer* renderer { nullptr };
	bool isRunning { false };
	std::list<State*> states;

	std::vector<State*> pendingOnEnterStates;
	std::vector<State*> pendingOnExitStates;

};

#endif