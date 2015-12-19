/*
 * Game header
 *
 *
 */

#ifndef GAME_H_
#define GAME_H_

//Constants
#include "Define.h"

//Graphics and Sound
#include <SDL.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

//Standard Library dependecies
#include <list>
#include <map>
#include <vector>
#include <sstream>

//Image related
#include <Entity/Sprite.h>
#include <Entity/Tower/Tower.h>
#include <Entity/Tower/BaseTower.h>
#include <Entity/Projectile.h>
#include <UI/Text.h>
#include <UI/Button.h>
#include <Entity/Enemy.h>

#include <UI/Sound.h>
#include <Utils/Timer.h>

//Pathfinding and Difficulty Control
#include <Pathfinding/Grid.h>
#include <Pathfinding/Dijkstra.h>
#include <Core/Level.h>

#include <Core/ConfigFile.h>

class Level;
class Timer;
class State;
class MainMenuState;
class IntroState;
class ViewHelpState;
class HighscoreState;
class GameOverState;
class InGameMenuState;
class GamePlayState;
typedef std::vector<std::pair<int, std::string>* > HighscoreList;
typedef std::pair<int, int> GridPosition;
typedef std::map<GridPosition, Sprite*> GridMap;

/** \brief The Game class.
 *
 * The game class controls the flow of the game. It connects different parts of the game.
 *
 * */
class Game {

friend class GameEngine;

public:
	Game(); ///Constructor
	virtual ~Game(); ///Destructor
	static TTF_Font* standard_font_48;	///Global font
	static TTF_Font* standard_font_46;	///Global font
	static TTF_Font* standard_font_42;	///Global font
	static TTF_Font* standard_font_32;	///Global font
	static TTF_Font* standard_font_20;	///Global font
	static TTF_Font* standard_font_18;	///Global font
	static TTF_Font* standard_font_16;	///Global font
	static TTF_Font* standard_font_12;	///Global font

	Grid* get_grid();

private:
	int current_fps;
	int timer;
	bool need_boost_update;	///< Indicates whether we need to update the boost connections
	bool update_enemy_path;		///Sets true if player build or sell a tower
	int lives;
	int score;
	int money;
	bool sound_enabled;
	bool music_enabled;
	int sound_volume;
	ConfigFile* config;
	void parseConfig();

	Sound* SFX_build;
	Sound* SFX_upgrade;
	Sound* SFX_sell;
	Sound* SFX_life_lost;
	Sound* SFX_new_highscore;
	Sound* SFX_game_over;
	Sound* music;

public:

// Variable to be moved from this class

// Game Variables
	Level* level_control;		//Controls difficulty and enemy waves

	void sendNewWave();
	void set_boost_update(bool);

	bool init();
	void reset_game();
	void cleanup();

	const int getScore() const { return score; }
	const int getMoney() const { return money; }
	const int getLives() const { return lives; }

	void upgrade_tower(BaseTower* const tower);
	void upgrade_tower(BaseTower* const tower, towers::TowerType type);
	void createNewTower(const towers::TowerType &type, const GridPosition &pos, Grid &grid);

	void setMusicPlaying(const bool play)
	{
		if (play)
		{
			music->play(); 
		}
		else
		{
			music->stop();
		}
	}

	/** Handle event depending on gamestate */
	bool sell(Tile* const);
	void update_boost();
	void get_rewards(Enemy* const enemy);
	void update(const float &timeStep);
	TowerList* get_towers();
	EnemyList* get_enemies();
	ProjectileList* get_projectiles();
	Grid* grid;

	GameEngine* const getEngine() const { return engine; }
	SDL_Renderer* const getRenderer() const;
private:
	GameEngine* engine;
public:

	/* STATES */
	IntroState* introState;
	MainMenuState* mainMenuState;
	ViewHelpState* viewHelpState;
	HighscoreState* highscoreState;
	GameOverState* gameOverState;
	InGameMenuState* inGameMenuState;
	GamePlayState* gamePlayState;

/** Sprite containers */

	TowerList tower_list;
	TowerList::iterator iter_tower;

	EnemyList enemy_list;
	EnemyList::iterator iter_enemy;

	ProjectileList projectile_list;
	ProjectileList::iterator iter_projectile;
};

#endif /* GAME_H_ */
