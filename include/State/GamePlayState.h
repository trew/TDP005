#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include <State/State.h>

class GamePlayState : public State
{
public:
	GamePlayState(Game* const game) : State(game) {}

	bool init() override;
	void cleanup() override;

	bool handleEvent(const SDL_Event &ev) override;
	void update(const float &timeStep) override;
	void render(SDL_Renderer* const renderer) override;

	// Simple Getters and Setters
	const int getSubState() const { return subState; } 
	void setSubState(const int state) { subState = state; }

	void setGridVisible(const bool visible) { gridVisible = visible; }
	const bool isGridVisible() const { return gridVisible; }

	// Actions
	void reset();
	void cancelSelection();

	const bool isOptionBoxVisible() const;
	void setOptionBoxVisible(const bool visible);
	void updateOptionBox();
	void composeOptionBoxPosition(int &optbox_pos_x, int &optbox_pos_y, Sprite* const option_box);
	bool optionBoxDoSelection(Sprite* const curr_opt_sel, const GridPosition &position);
	bool optionBoxDoSelection(const int type, const GridPosition &position);

	void select(Tile* const);
	void selectFromBuildMenu(BaseTower* const);


	void update_lives();
	void update_money();
	void update_score();
	void update_level();
	void update_timer();
	void updateFpsText();

private:
	int subState;
	float sound_btn_repeat_value;
	float sound_btn_repeat_delay;
	int old_timer;

	// Game Variables
	bool gameStarted;
	BaseTower* selectedTowerInBuildMenu;
	BaseTower* hoveredTowerInBuildMenu;
	Tile* selectedTile;
	Tile* lastSelectedTile;
	Sprite_List optionBox;
	bool optionBoxVisible;
	bool gridVisible;

	Sound* SFX_cant_build;

	Button* menuButton;

	// Sprites
	Text* speedText;
	Sprite* selectionMarker;
	Sprite* map;
	Sprite* mapGrid;
	Sprite* mapEntrance;
	Sprite* mapExit;
	Sprite* mapWall;
	Sprite* menuBackground;
	Sprite* menuMoneyScore;
	Sprite* menuBuild;
	Sprite* menuUpgrade;
	Sprite* menuInfo;
	Sprite* menuLives;
	Text* pressEnterToStartText;
	Text* livesText;
	Text* scoreText;
	Text* moneyText;
	Text* levelText;
	Text* splitMoneyScore;
	Sprite* fpsText;
	Timer fpsTimer;
	int oldTimer;
	Text* timerText;
	Sprite* availableTileMarker;
	Sprite* occupiedTileMarker;

	/** Different sizes on box depending on amount of buttons in it */
	Sprite* optionBoxBGx1;
	Sprite* optionBoxBGx2;
	Sprite* optionBoxBGx3;
	Sprite* optionBoxBGx4;
	Sprite* optionBoxBGx5;
	Sprite* optionBoxBGx6;

	// Lists
	TowerList buildList; // TODO what is this?
	Button* soundButton;
	ButtonList inGameButtons;
	std::map<int, Sprite*> optionBoxButtonStorage;

};

#endif