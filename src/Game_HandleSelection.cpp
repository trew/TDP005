/*
 * Game_HandleSelection.cpp
 *
 *  Created on: 29 nov 2010
 *      Author: samuel
 */

#include "Game.h"

bool Game::sell(Sprite* selection)
{
	//Tries to sell empty spot
	if (selection == NULL)
		return false;

	else if (grid_control[get_grid_position(selection->get_x_pos(), selection->get_y_pos())] == NULL)
	{
		return false;
	}
	else
	{
		if(game_state == GAME_PAUSED)
			money += selection->get_sell_value();
		else
			money += selection->get_sell_value() / 2;


		update_money();
		SFX_sell->play();
		update_enemy_path = true;
		tower_list.remove(grid_control[get_grid_position(selection->get_x_pos(), selection->get_y_pos())]);
		grid_control[get_grid_position(selection->get_x_pos(), selection->get_y_pos())] = NULL;
		delete grid_control[get_grid_position(selection->get_x_pos(), selection->get_y_pos())];

		for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
		{
			(*iter_enemy)->can_update_path();
		}
		clear_selectioninfo();
		current_selection = NULL;
		hide_option_box();
	}
	return true;
}

void Game::cancel_selection()
{
	update_option_box();
	if (current_selection != NULL)
		current_selection->clear_selected();
	current_selection = NULL;
	building_flag = false;
	selection_sprite->hide();
	hide_option_box();
	clear_selectioninfo();

}

void Game::clear_selectioninfo()
{
	selection.clear();
}

void Game::set_selection_info(Sprite* selection_ptr)
{
	clear_selectioninfo();
	selection = selection_ptr->get_infosprites();

}

void Game::select(Sprite* selected_sprite)
{
	if (current_selection != NULL)
		current_selection->clear_selected();
	current_selection = selected_sprite;

	if (!building_flag)
		selection_sprite->show();
	selection_sprite->set_x_pos(current_selection->get_x_pos() - 2);
	selection_sprite->set_y_pos(current_selection->get_y_pos() - 2);
	current_selection->set_selected();
	update_option_box();

	set_selection_info(current_selection);
}
