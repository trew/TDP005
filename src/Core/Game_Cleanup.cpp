/*
 * Game_Cleanup.cpp
 *
 *  Created on: 29 nov 2010
 *      Author: samuel
 */

#include <Core/Game.h>

void Game::reset_game()
{
	grid->clear_paths();
	grid->reset();// needs to be done before deleting towers

	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		delete (*iter_enemy);
		(*iter_enemy) = NULL;
	}
	enemy_list.clear();

	for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
	{
		delete (*iter_tower);
		(*iter_tower) = NULL;
	}
	tower_list.clear();

	for (iter_projectile = projectile_list.begin(); iter_projectile != projectile_list.end(); iter_projectile++)
	{
		delete (*iter_projectile);
		(*iter_projectile) = NULL;
	}
	projectile_list.clear();

	selection_sprite->set_x(-5);
	selection_sprite->set_y(-5);
	selection_sprite->hide();

	level_control->reset();

	tile_selection = NULL;
	buildmenu_selection = NULL;
	option_box_visible = false;
	money = STARTING_MONEY;
	score = 0;
	lives = STARTING_LIVES;
	playername = "";
	input_text->update_text("Enter your name");
	game_started = false;

	update_score();
	update_money();
	update_lives();
	update_level();
}

void Game::cleanup()
{
	config->set_value<bool>("fullscreen", fullscreen);
	config->set_value<int>("sound_volume", Sound::get_volume());
	config->set_value<bool>("grid", grid_visible);
	config->save();
	delete config;

	Mix_CloseAudio();

	delete level_control;
	delete grid;

	delete map;
	delete map_grid;
	delete map_wall;
	delete map_entrance;
	delete map_exit;

	delete dev_screen;
	delete intro_screen;
	delete introduction_screen;
	delete highscore_screen;
	delete ingame_menu_screen;
	delete gameover_screen;

	delete menu_background;
	delete menu_money_score;
	delete menu_build;
	delete menu_upgrade;
	delete menu_info;
	delete menu_lives;
	delete selection_sprite;

	delete fps_timer;
	delete delta_timer;

	delete press_enter_to_start;
	delete error_loading_highscore;
	delete esc_back;
	delete gameover_score_text;

	delete free_spot;
	delete not_free_spot;

	delete score_text;
	delete lives_text;
	delete money_text;
	delete level_text;
	delete timer_text;
	delete fps_text;
	delete speed_text;
	delete input_text;
	delete split_money_score;

	delete option_box_BGx1;
	delete option_box_BGx2;
	delete option_box_BGx3;
	delete option_box_BGx4;
	delete option_box_BGx5;
	delete option_box_BGx6;

	for (iter_map = optionbox_buttonstorage.begin(); iter_map != optionbox_buttonstorage.end(); iter_map++)
	{
		delete (*iter_map).second;
	}

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

	//Delete available towers on menu
	for (iter_build_obj = build_list.begin(); iter_build_obj != build_list.end(); iter_build_obj++)
	{
		delete (*iter_build_obj);
	}

	//Delete buttons
	for (iter_ingame_button = ingame_buttons.begin(); iter_ingame_button != ingame_buttons.end(); iter_ingame_button++)
	{
		delete (*iter_ingame_button);
	}

	//Delete ingame menu buttons
	for (iter_ingame_menu_button = ingame_menu_buttons.begin(); iter_ingame_menu_button != ingame_menu_buttons.end(); iter_ingame_menu_button++)
	{
		delete (*iter_ingame_menu_button);
	}

	//Delete buttons
	mainMenuState->cleanup();
	delete mainMenuState;

	//Delete highscore-sprites
	for (iter_highscore = highscores.begin(); iter_highscore != highscores.end(); iter_highscore++)
	{
		delete (*iter_highscore);
		(*iter_highscore) = NULL;
	}

	for (iter_highscore_name = highscore_name_sprites.begin(); iter_highscore_name != highscore_name_sprites.end(); iter_highscore_name++)
	{
		delete (*iter_highscore_name);
	}

	for (iter_highscore_score = highscore_score_sprites.begin(); iter_highscore_score != highscore_score_sprites.end(); iter_highscore_score++)
	{
		delete (*iter_highscore_score);
	}


	delete music;
	delete SFX_cant_build;
	delete SFX_build;
	delete SFX_sell;
	delete SFX_upgrade;
	delete SFX_life_lost;
	delete SFX_new_highscore;
	delete SFX_game_over;

	SDL_DestroyRenderer(renderer);

	TTF_CloseFont(standard_font_48);
	TTF_CloseFont(standard_font_46);
	TTF_CloseFont(standard_font_42);
	TTF_CloseFont(standard_font_20);
	TTF_CloseFont(standard_font_18);
	TTF_CloseFont(standard_font_16);
	TTF_CloseFont(standard_font_12);

	TTF_Quit();
	IMG_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}
