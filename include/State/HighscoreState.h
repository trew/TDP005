#ifndef HIGHSCORESTATE_H
#define HIGHSCORESTATE_H

#include <State/State.h>

const int SHOW = 1;
const int ENTER = 2;

class HighscoreState : public State
{
public:
	HighscoreState(Game* const game) : State(game) {}

	bool init() override;
	void cleanup() override;

	void onEnter() override;
	void onExit() override;

	bool handleEvent(const SDL_Event &ev) override;
	void render(SDL_Renderer* const renderer) override;

	bool isQualifyingScore(const int score);
	void updateHighscoreSprites();
	void setInternalState(const int state) { internalState = state; }

	void setScore(const int score);
private:
	int internalState;

	Sprite* highscoreScreen;
	std::vector<Sprite*> highscoreNameSprites;
	std::vector<Sprite*> highscoreScoreSprites;
	Sprite* gameoverScoreText;
	Sprite* inputText;
	std::string playerName;
	HighscoreList highscores;

	bool readHighscoresFromFile();
	const int getHighscorePos(const int);
	void insertNewHighscore(int newScore, int position, std::string name);
	void writeHighscoreToFile();
};

#endif