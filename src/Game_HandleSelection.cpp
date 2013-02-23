/*
 * Game_HandleSelection.cpp
 *
 *  Created on: 29 nov 2010
 *      Author: samuel
 */

#include "Game.h"

bool Game::sell(Tile* tile)
{
	if (tile == NULL)
		return false;
	Tower* tower = tile->get_tower();

	//Tries to sell empty spot
	if (tower == NULL)
		return false;
	else
	{
		if(game_state == GAME_PAUSED)
			money += tower->get_sell_value();
		else
			money += tower->get_sell_value() / 2;

		update_money();
		SFX_sell->play();
		update_enemy_path = true;
		tower->get_tile()->set_tower(NULL);
		tower_list.remove(tower);
		delete tower;
		tower = NULL;

		grid->clear_paths();
		for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
		{
			(*iter_enemy)->try_update_path(grid);
		}
		clear_selectioninfo();
		tile_selection = NULL;
		hide_option_box();
	}
	return true;
}

void Game::cancel_selection()
{
	update_option_box();
	if (tile_selection != NULL && tile_selection->get_tower() != NULL)
		tile_selection->get_tower()->clear_selected();
	if (buildmenu_selection != NULL) {
		buildmenu_selection->clear_selected();
	}
	tile_selection = NULL;
	buildmenu_selection = NULL;
	building_flag = false;
	selection_sprite->hide();
	hide_option_box();
	clear_selectioninfo();

}

void Game::clear_selectioninfo()
{
	selection_text.clear();
}

void Game::set_selection_info(Tower* selection_ptr)
{
	clear_selectioninfo();
	if (selection_ptr != NULL)
		selection_text = selection_ptr->get_infosprites();

}

void Game::select(Tile* tile)
{
	Tower* tower = NULL;
	if (tile_selection != NULL)
		tower = tile_selection->get_tower();

	if (buildmenu_selection != NULL) {
		buildmenu_selection->clear_selected();
		buildmenu_selection = NULL;
	}
	tile_selection = tile;
	last_selected_tile = tile_selection;
	if (tower != NULL)
		tower->clear_selected();

	tower = tile_selection->get_tower();

	selection_sprite->show();
	selection_sprite->set_x_pos(tile_selection->get_x_pixel_pos() - 2);
	selection_sprite->set_y_pos(tile_selection->get_y_pixel_pos() - 2);
	if (tower != NULL)
		tower->set_selected();
	update_option_box();

    set_selection_info(tower);
}

void Game::select_from_buildmenu(Tower* tower) {
	if (tile_selection != NULL && tile_selection->get_tower() != NULL) {
		tile_selection->get_tower()->clear_selected();
		tile_selection = NULL;
	}

	buildmenu_selection = tower;
	selection_sprite->show();
	selection_sprite->set_x_pos(buildmenu_selection->get_x_pos() - 2);
	selection_sprite->set_y_pos(buildmenu_selection->get_y_pos() - 2);
	tower->set_selected();
	update_option_box();
	set_selection_info(tower);
}
