#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <State/State.h>

class GameOverState : public State
{
public:
	GameOverState(Game* const game) : State(game) {}

	bool init();
	void cleanup();

	bool handleEvent(const SDL_Event &ev);
	void render(SDL_Renderer* const renderer);

	void setScore(const int score);
private:
	Sprite* gameOverScreen;
	Sprite* gameOverScoreText;
};

#endif