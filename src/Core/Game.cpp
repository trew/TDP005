#include <Core/Game.h>
#include <Utils/Utils.h>
#include <State/State.h>
#include <State/MainMenuState.h>
#include <State/IntroState.h>
#include <State/ViewHelpState.h>
#include <State/HighscoreState.h>
#include <State/GameOverState.h>
#include <State/InGameMenuState.h>
#include <State/GamePlayState.h>
#include <Core/GameEngine.h>
#include <Utils/Log.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

TTF_Font* Game::standard_font_48;
TTF_Font* Game::standard_font_46;
TTF_Font* Game::standard_font_42;
TTF_Font* Game::standard_font_32;
TTF_Font* Game::standard_font_20;
TTF_Font* Game::standard_font_18;
TTF_Font* Game::standard_font_16;
TTF_Font* Game::standard_font_12;


//TODO Click here to come to the great TO-DO-list!

//TODO 4/12  -10	Leta buggar(bara F11) med extrema värden. Sätt startpengar till 10000 t.ex.
//TODO 5/12  -10	Different music for Menues and gameplay. Intromusic?
//TODO 6/12  -10	Show the money earned when killing an enemy
//TODO 7/12  -10    Set all kinds of pointers to NULL in game constructor (so they are nullpointers until Init is run

Game::Game()
{
	level_control = new Level();
	update_enemy_path = false;
	need_boost_update = true;
	timer = 0;
	current_fps = 0;
	sound_volume = 0;
	config = NULL;

	/* Flags */
	music_enabled = true;
	sound_enabled = true;

	money = STARTING_MONEY;
	score = 0;
	lives = STARTING_LIVES;

	// initialize pointers to null
	SFX_build = SFX_game_over = SFX_life_lost = SFX_new_highscore = SFX_sell = SFX_upgrade = music = NULL;
}

Game::~Game(){
	///Nothing needed in destructor. Cleanup is run at end of game.
}

void Game::parseConfig()
{
	LOG_INFO << "Parsing config file";
	config = new ConfigFile("settings.cfg");
	sound_volume = config->getValue<int>("sound_volume", 35);
}

bool Game::init()
{
	LOG_INFO << "Initializing Game";
	parseConfig();

	std::string standard_font = "./fonts/Graffiare.ttf";

	standard_font_48 = TTF_OpenFont(standard_font.c_str(), 48);
	standard_font_46 = TTF_OpenFont(standard_font.c_str(), 46);
	standard_font_42 = TTF_OpenFont(standard_font.c_str(), 42);
	standard_font_32 = TTF_OpenFont(standard_font.c_str(), 32);
	standard_font_20 = TTF_OpenFont(standard_font.c_str(), 20);
	standard_font_18 = TTF_OpenFont(standard_font.c_str(), 18);
	standard_font_16 = TTF_OpenFont(standard_font.c_str(), 16);
	standard_font_12 = TTF_OpenFont(standard_font.c_str(), 12);

	//Load sounds

	SFX_build = new Sound("./snd/SFX_4.wav", false, 0);
	SFX_sell = new Sound("./snd/SFX_5.wav", false, 0);
	SFX_upgrade = new Sound("./snd/SFX_2.wav", false, 0);
	SFX_life_lost = new Sound("./snd/SFX_3.wav", false, 0);
	SFX_new_highscore = new Sound("./snd/new_highscore.wav", false, 0);
	SFX_game_over = new Sound("./snd/game_over.wav", false, 0);

	grid = new Grid;
	grid->create_grid(15, 15);
	grid->set_portal_tile(grid->get_tile(GridPosition(7, 14)));
	grid->set_start_tile(grid->get_tile(GridPosition(7, 0)));

	//Main menu buttons
	mainMenuState = new MainMenuState(this);
	mainMenuState->init();
	introState = new IntroState(this);
	introState->init();
	viewHelpState = new ViewHelpState(this);
	viewHelpState->init();
	highscoreState = new HighscoreState(this);
	highscoreState->init();
	gameOverState = new GameOverState(this);
	gameOverState->init();
	inGameMenuState = new InGameMenuState(this);
	inGameMenuState->init();
	gamePlayState = new GamePlayState(this);
	gamePlayState->init();
	gamePlayState->setGridVisible(config->getValue<bool>("grid", true));
	getEngine()->setState(introState);

	music = new Sound("./snd/Ultrasyd_Lonesome_Robot.ogg", true, -1);
	Sound::setVolume(sound_volume);
	if (music_enabled)
	{
		music->play();
	}

	return true;
}

void Game::reset_game()
{
	LOG_INFO << "Resetting Game";

	LOG_DEBUG << "Clearing paths in grid";
	grid->clear_paths();
	LOG_DEBUG << "Resetting grid";
	grid->reset();// needs to be done before deleting towers

	LOG_DEBUG << "Clearing enemies";
	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		delete (*iter_enemy);
		(*iter_enemy) = NULL;
	}
	enemy_list.clear();

	LOG_DEBUG << "Clearing towers";
	for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
	{
		delete (*iter_tower);
		(*iter_tower) = NULL;
	}
	tower_list.clear();

	LOG_DEBUG << "Clearing projectiles";
	for (iter_projectile = projectile_list.begin(); iter_projectile != projectile_list.end(); iter_projectile++)
	{
		delete (*iter_projectile);
		(*iter_projectile) = NULL;
	}
	projectile_list.clear();

	LOG_DEBUG << "Resetting level control";
	level_control->reset();

	money = STARTING_MONEY;
	score = 0;
	lives = STARTING_LIVES;
	LOG_DEBUG << "Resetting gameplaystate";
	gamePlayState->reset();
}

void Game::cleanup()
{
	LOG_INFO << "Destroying Game";

	LOG_INFO << "Saving Config File";
	config->setValue<bool>("fullscreen", false);
	config->setValue<int>("sound_volume", Sound::getVolume());
	config->setValue<bool>("grid", gamePlayState->isGridVisible());
	config->save();
	delete config;

	delete music;

	delete level_control;
	delete grid;

	//Delete Enemies on Grid
	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		delete (*iter_enemy);
	}

	//Delete Towers on Grid
	for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
	{
		delete (*iter_tower);
	}

	//Delete Towers on Grid
	for (iter_projectile = projectile_list.begin(); iter_projectile != projectile_list.end(); iter_projectile++)
	{
		delete (*iter_projectile);
	}

	//Delete buttons
	mainMenuState->cleanup();
	delete mainMenuState;
	introState->cleanup();
	delete introState;
	viewHelpState->cleanup();
	delete viewHelpState;
	highscoreState->cleanup();
	delete highscoreState;
	gameOverState->cleanup();
	delete gameOverState;
	inGameMenuState->cleanup();
	delete inGameMenuState;
	gamePlayState->cleanup();
	delete gamePlayState;

	delete SFX_build;
	delete SFX_sell;
	delete SFX_upgrade;
	delete SFX_life_lost;
	delete SFX_new_highscore;
	delete SFX_game_over;

	TTF_CloseFont(standard_font_48);
	TTF_CloseFont(standard_font_46);
	TTF_CloseFont(standard_font_42);
	TTF_CloseFont(standard_font_20);
	TTF_CloseFont(standard_font_18);
	TTF_CloseFont(standard_font_16);
	TTF_CloseFont(standard_font_12);
}


void Game::set_boost_update(bool val) {
	need_boost_update = val;
}

Grid* Game::get_grid() {
	return grid;
}

TowerList* Game::get_towers() {
	return &tower_list;
}

EnemyList* Game::get_enemies() {
	return &enemy_list;
}

ProjectileList* Game::get_projectiles() {
	return &projectile_list;
}

SDL_Renderer* const Game::getRenderer() const { return engine->getRenderer(); }

void Game::sendNewWave()
{
	if (DEBUGMODE)
	{
		return;
	}

	EnemyList tmp = level_control->get_new_wave(this);
	get_enemies()->insert(get_enemies()->end(), tmp.begin(), tmp.end());
	tmp.clear();
}
