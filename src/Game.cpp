#include "Game.h"
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
	level_control = new Level;
	FPS_MAX = 100;						//Defines max FPS. Slow computers may experience lower FPS though.
	fullscreen = false;
	game_running = true;
	game_state = DEVSCREEN;			//In which state we start the game.
	game_started = false;			//If false, display "Press enter to start"
	tile_selection = NULL;
	last_selected_tile = NULL;
	update_enemy_path = false;
	need_boost_update = true;
	old_game_state = 0;
	timer = 0;
	old_timer = 0;
	current_fps = 0;
	fps_timer = delta_timer = NULL;
	delta = 0;
	game_speed = 1.f;
	sound_volume = 0;
	config = NULL;

	/* Flags */
	grid_visible = true;
	option_box_visible = false;
	music_enabled = true;
	sound_enabled = true;
	sound_btn_repeat_delay = 100;
	sound_btn_repeat_value = sound_btn_repeat_delay;

	money = STARTING_MONEY;
	score = 0;
	lives = STARTING_LIVES;
	playername = ""; 	//Set when entering highscore

	// initialize pointers to null
	SFX_build = SFX_cant_build = SFX_game_over = SFX_life_lost = SFX_new_highscore = SFX_sell = SFX_upgrade = music = NULL;
	menu_build = menu_background = split_money_score = menu_money_score = menu_upgrade = menu_lives = menu_info = NULL;
	option_box_BGx1 = option_box_BGx2 = option_box_BGx3 = option_box_BGx4 = option_box_BGx5 = option_box_BGx6 = NULL;
	selection_sprite = free_spot = not_free_spot = NULL;
	map_exit = map_wall = map_grid = map_entrance = map = NULL;
	grid = NULL;
	fps_text = timer_text = NULL;
	intro_screen = highscore_screen = ingame_menu_screen = main_menu_screen = dev_screen = introduction_screen = gameover_screen = NULL;
	press_enter_to_start = NULL;
	error_loading_highscore = NULL;
	input_text = lives_text = level_text = money_text = gameover_score_text = score_text = NULL;
	buildmenu_selection = NULL;
	screen = NULL;
	esc_back = NULL;
	hovered_build_item = NULL;
	sound_button = NULL;
}

Game::~Game(){
	///Nothing needed in destructor. Cleanup is run at end of game.
}

void Game::set_boost_update(bool val) {
	need_boost_update = val;
}

Grid* Game::get_grid() {
	return grid;
}

bool Game::read_highscores_from_file()
{
	if (!highscores.empty())
		highscores.clear();

	std::ifstream file_in;
	file_in.open("highscore");
	if (!file_in)
		return false;

	int score;
	std::string name;

	file_in >> score;
	file_in.get();
	while (getline(file_in, name))
	{
		highscores.push_back(new std::pair<int, std::string>(score, name));
		file_in >> score;
		file_in.get();
	}

	file_in.close();
	return true;
}

int Game::get_highscore_pos()
{
	if(highscores.empty()) return 0;

	for (unsigned int i = 0; i < 10 && i < highscores.size(); i++)
	{
		if (score > highscores[i]->first)
		{
			return i;
		}
	}

	if(highscores.size() < 10)
		return highscores.size();

	else
		return -1;
}

void Game::insert_new_highscore(int new_score, int position, std::string name)
{


	iter_highscore = highscores.begin();

	for (int i = 0; i < position; i++)
		iter_highscore++;

	highscores.insert(iter_highscore, new std::pair<int, std::string>(new_score, name));
}

void Game::write_highscore_to_file()
{
	std::ofstream fout;
	fout.open("highscore");

	int score_count = 0;
	for (iter_highscore = highscores.begin(); iter_highscore != highscores.end(); iter_highscore++)
	{
		if (score_count == 10)
			break;

		fout << (*iter_highscore)->first << " " <<(*iter_highscore)->second << std::endl;

		score_count++;
	}
}

void Game::update_highscore_sprites()
{
	// Delete earlier sprites
	if(!highscore_name_sprites.empty())
		for(iter_highscore_name = highscore_name_sprites.begin(); iter_highscore_name != highscore_name_sprites.end(); iter_highscore_name++)
		{
			delete (*iter_highscore_name);
		}

	if(!highscore_score_sprites.empty())
		for(iter_highscore_score = highscore_score_sprites.begin(); iter_highscore_score != highscore_score_sprites.end(); iter_highscore_score++)
		{
			delete (*iter_highscore_score);
		}


	highscore_name_sprites.clear();
	highscore_score_sprites.clear();

	//Error check!
	if(!read_highscores_from_file()) {
		error_loading_highscore->show();
		return;
	}
	error_loading_highscore->hide();

	std::string tmp_name = "";
	std::string tmp_score = "";

	int list_num = 1;
	iter_highscore = highscores.begin();
	iter_highscore_name = highscore_name_sprites.begin();
	iter_highscore_score = highscore_score_sprites.begin();
	while(iter_highscore != highscores.end())
	{
		{
			std::ostringstream oss_name;
			oss_name << itos(list_num) << ". " << (*iter_highscore)->second;
			tmp_name = oss_name.str();

			std::ostringstream oss_score;
			oss_score << (*iter_highscore)->first;
			tmp_score = oss_score.str();
		}
		if(list_num == 1) {
			highscore_name_sprites.push_back(new Text(tmp_name, 186, 255, 246, 0, 0, standard_font_48));
			highscore_score_sprites.push_back(new Text(tmp_score, 186, 255, 246, 0, 0, standard_font_48));
		}
		else
		{
			highscore_name_sprites.push_back(new Text(tmp_name, 167, 203, 237, 0, 0, standard_font_20));
			highscore_score_sprites.push_back(new Text(tmp_score, 167, 203, 237, 0, 0, standard_font_20));
		}



		list_num++;
		iter_highscore++;
		iter_highscore_name++;
		iter_highscore_score++;
	}
}

void Game::show_intro(SDL_Event* event)
{
	unsigned int starttime = SDL_GetTicks();
	while (game_state == DEVSCREEN)
	{
		render();
		while (SDL_PollEvent(event))
		{
			if (event->type == SDL_QUIT)
			{
				game_running = false;
				game_state = EXIT;
				break;
			}
			else if (event->type == SDL_KEYDOWN)
			{
				if (event->key.keysym.sym == SDLK_ESCAPE)
					game_state = INTROSCREEN;
				if (event->key.keysym.sym == SDLK_SPACE)
					game_state = INTROSCREEN;
				if (event->key.keysym.sym == SDLK_F12)
					toggle_fullscreen();
			}
		}

		//Automaticly switch to Introscreen
		if (SDL_GetTicks() > (starttime + 6000) && game_state == DEVSCREEN)
			game_state = INTROSCREEN;
	}

	while (game_state == INTROSCREEN)
	{
		render();
		while (SDL_PollEvent(event))
		{
			if (event->type == SDL_QUIT)
			{
				game_running = false;
				game_state = EXIT;
				break;
			}
			else if (event->type == SDL_KEYDOWN)
				game_state = MAINMENU;
		}
	}
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

float Game::get_time_modifier() {
	return delta / 1000.f * game_speed;
}

float Game::get_game_speed() {
	return game_speed;
}

void Game::update_fps(int delta) {
	if ( fps_timer->get_ticks() < 1000) {
		current_fps++;
	} else {
		fps_timer->start();
		std::string tmp = "FPS: ";
		tmp.append(itos(current_fps));
		tmp.append(" - Delta: ");
		tmp.append(itos(delta));
		fps_text->update_text(tmp);
		current_fps = 0;
	}
}

int Game::on_execute()
{
	/**
	 * This is the function that runs the game.
	 * It contains the initializing phase, the game loop and finally the cleanup process.
	 */
	if (init() == false)
		return -1;

	SDL_Event event;

	/* Introscreen */
	show_intro(&event);

	/* Game */
	fps_timer->start();
	delta_timer->start();
	while (game_running)
	{
		delta = delta_timer->get_ticks();
		delta_timer->start();
		while (SDL_PollEvent(&event))
		{
			handle_event(&event);
		}
		update((int)(delta * game_speed));

		update_fps(delta);
		render();

		if (delta_timer->get_ticks() < (Uint32)(1000 / FPS_MAX)) {
			SDL_Delay( (1000 / FPS_MAX) - delta_timer->get_ticks());
		}
	}
	/* End Game */

	cleanup();
	return 0;
}

