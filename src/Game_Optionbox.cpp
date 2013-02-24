/*
 * Game_Optionbox.cpp
 *
 *  Created on: 29 nov 2010
 *      Author: samuel
 */

#include "Game.h"



void Game::compose_box_pos(int &optbox_pos_x, int &optbox_pos_y, Sprite *optionbox)
{
	int box_offset_x = 25;
	int box_offset_y = -25;
	if (tile_selection == NULL)
	{
		optbox_pos_x = (int)selection_sprite->get_x_pos() + box_offset_x;
		optbox_pos_y = (int)selection_sprite->get_y_pos() + box_offset_y;
	}
	else
	{
		optbox_pos_x = tile_selection->get_x_pixel_pos() + box_offset_x;
		optbox_pos_y = tile_selection->get_y_pixel_pos() + box_offset_y;
	}

	if ((optbox_pos_x + optionbox->get_width()) > GRIDWIDTH)
		optbox_pos_x -= (optbox_pos_x + optionbox->get_width()) - GRIDWIDTH;

	if ((optbox_pos_y + optionbox->get_height()) > WHEIGHT)
		optbox_pos_y -= (TILESIZE + optionbox->get_height());

	if (optbox_pos_x < 0)
		optbox_pos_x = 0;

	if (optbox_pos_y < 0)
		optbox_pos_y = -box_offset_y;

	optionbox->set_x_pos(optbox_pos_x);
	optionbox->set_y_pos(optbox_pos_y);
}

void Game::toggle_option_box()
{
	if (option_box_visible)
	{
		hide_option_box();
	}
	else
	{
		show_option_box();
	}
}

void Game::update_option_box()
{
	int button_spacing = 27;
	int button_offset_x = 6;
	int button_offset_y = 6;

	optionbox.clear();
	int optbox_pos_x;
	int optbox_pos_y;

	if (tile_selection == NULL || tile_selection->get_tower() == NULL)
	{
		optionbox.push_back(option_box_BGx3);
		if (selection_sprite->is_visible())
		{
			compose_box_pos(optbox_pos_x, optbox_pos_y, option_box_BGx3);
			int button_x = optbox_pos_x + button_offset_x;
			int button_y = optbox_pos_y + button_offset_y;

			optionbox.push_back(optionbox_buttonstorage[BUTTON_BASE]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);

			optionbox.push_back(optionbox_buttonstorage[BUTTON_BOOST]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);

			optionbox.push_back(optionbox_buttonstorage[BUTTON_WALL]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);
		}
	}

	else if (tile_selection != NULL && tile_selection->get_tower() != NULL)
	{
		if (tile_selection->get_tower()->get_type() == TOWER_BASE)
		{
			optionbox.push_back(option_box_BGx5);
			compose_box_pos(optbox_pos_x, optbox_pos_y, option_box_BGx5);
			int button_x = optbox_pos_x + button_offset_x;
			int button_y = optbox_pos_y + button_offset_y;

			//Push back the 4 upgrade-buttons + sell-button, offset px apart from each other
			optionbox.push_back(optionbox_buttonstorage[BUTTON_BASIC]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);

			optionbox.push_back(optionbox_buttonstorage[BUTTON_SPEED]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);

			optionbox.push_back(optionbox_buttonstorage[BUTTON_RANGE]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);

			optionbox.push_back(optionbox_buttonstorage[BUTTON_BOMB]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);

			optionbox.push_back(optionbox_buttonstorage[BUTTON_SELL]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);
		}

		// If a wall is selected
		if (tile_selection->get_tower()->get_type() == TOWER_WALL)
		{
			optionbox.push_back(option_box_BGx1);
			compose_box_pos(optbox_pos_x, optbox_pos_y, option_box_BGx1);
			int button_x = optbox_pos_x + button_offset_x;
			int button_y = optbox_pos_y + button_offset_y;

			//Push back a Sell-button
			optionbox.push_back(optionbox_buttonstorage[BUTTON_SELL]);
			optionbox.back()->set_x_pos(button_x);
			button_x += button_spacing;
			optionbox.back()->set_y_pos(button_y);
		}

		// If a tower is selected
		else if (tile_selection->get_tower()->get_type() >= TOWER_BASIC_LEVEL_1 && tile_selection->get_tower()->get_type() < ENEMY)
		{

			// If tower is level 3, it cant be upgraded more.
			if (tile_selection->get_tower()->get_level() == 3)
			{
				optionbox.push_back(option_box_BGx2);
				compose_box_pos(optbox_pos_x, optbox_pos_y, option_box_BGx2);
				int button_x = optbox_pos_x + button_offset_x;
				int button_y = optbox_pos_y + button_offset_y;

				optionbox.push_back(optionbox_buttonstorage[BUTTON_NOUPGRADE]);
				optionbox.back()->set_x_pos(button_x);
				button_x += button_spacing;
				optionbox.back()->set_y_pos(button_y);

				optionbox.push_back(optionbox_buttonstorage[BUTTON_SELL]);
				optionbox.back()->set_x_pos(button_x);
				button_x += button_spacing;
				optionbox.back()->set_y_pos(button_y);

			}

			else
			{ // If tower is level1 or 2, and can be upgraded..
				optionbox.push_back(option_box_BGx2);
				compose_box_pos(optbox_pos_x, optbox_pos_y, option_box_BGx2);
				int button_x = optbox_pos_x + button_offset_x;
				int button_y = optbox_pos_y + button_offset_y;

				optionbox.push_back(optionbox_buttonstorage[BUTTON_UPGRADE]);
				optionbox.back()->set_x_pos(button_x);
				button_x += button_spacing;
				optionbox.back()->set_y_pos(button_y);

				optionbox.push_back(optionbox_buttonstorage[BUTTON_SELL]);
				optionbox.back()->set_x_pos(button_x);
				button_x += button_spacing;
				optionbox.back()->set_y_pos(button_y);
			}
		}
	}
}

void Game::show_option_box()
{
	if (selection_sprite->is_visible())
	{
		option_box_visible = true;
		update_option_box();
	}
}

void Game::hide_option_box()
{
	option_box_visible = false;
}

bool Game::optionbox_visible() {
	return option_box_visible;
}
