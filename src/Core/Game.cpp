#include <Core/Game.h>
#include <Utils/Utils.h>
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

	// initialize pointers to null
	SFX_build = SFX_cant_build = SFX_game_over = SFX_life_lost = SFX_new_highscore = SFX_sell = SFX_upgrade = music = NULL;
	menu_build = menu_background = split_money_score = menu_money_score = menu_upgrade = menu_lives = menu_info = NULL;
	option_box_BGx1 = option_box_BGx2 = option_box_BGx3 = option_box_BGx4 = option_box_BGx5 = option_box_BGx6 = NULL;
	selection_sprite = free_spot = not_free_spot = NULL;
	map_exit = map_wall = map_grid = map_entrance = map = NULL;
	grid = NULL;
	fps_text = timer_text = speed_text = NULL;
	ingame_menu_screen = gameover_screen = NULL;
	press_enter_to_start = NULL;
	error_loading_highscore = NULL;
	lives_text = level_text = money_text = score_text = NULL;
	buildmenu_selection = NULL;
	window = NULL;
	renderer = NULL;
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

void Game::update_fps(int delta, int ev, int upd, int ren) {
	if ( fps_timer->get_ticks() < 1000) {
		current_fps++;
	} else {
		fps_timer->start();
		std::string tmp = "FPS: ";
		tmp.append(Utils::itos(current_fps));
		tmp.append(" - Delta: ");
		tmp.append(Utils::itos(delta));
		tmp.append(" - Event: ");
		tmp.append(Utils::itos(ev));
		tmp.append(" - Update: ");
		tmp.append(Utils::itos(upd));
		tmp.append(" - Render: ");
		tmp.append(Utils::itos(ren));
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
	{
		cleanup();
		return -1;
	}

	SDL_Event event;

	/* Game */
	fps_timer->start();
	delta_timer->start();
	int ev = 0;
	int upd = 0;
	int ren = 0;

	while (game_running)
	{
		delta = delta_timer->get_ticks();
		delta_timer->start();
		while (SDL_PollEvent(&event))
		{
			handle_event(&event);
			ev = delta_timer->get_ticks();
		}
		update((int)(delta * game_speed));
		upd = delta_timer->get_ticks() - ev;

		update_fps(delta, ev, upd, ren);
		render();
		ren = delta_timer->get_ticks() - upd;

		if (delta_timer->get_ticks() < (Uint32)(1000.f / FPS_MAX)) {
			SDL_Delay( (1000.f / FPS_MAX) - delta_timer->get_ticks());
		}
	}
	/* End Game */

	cleanup();
	return 0;
}

void Game::setState(int state)
{
	game_state = state;
}

void Game::exit()
{
	game_running = false;
}

