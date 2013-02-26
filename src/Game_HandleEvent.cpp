/*
 * Game_HandleEvent.cpp
 *
 *  Created on: Nov 21, 2010
 *      Author: samuel
 */

#include "Game.h"
#include <string>

bool Game::is_arrow_key(SDL_Event* event)
{
	return ((event->key.keysym.sym == SDLK_LEFT) || (event->key.keysym.sym == SDLK_RIGHT) || (event->key.keysym.sym == SDLK_UP) || (event->key.keysym.sym
			== SDLK_DOWN));
}

void Game::create_new_tower(towers::TowerType tower_type, GridPosition position)
{
	Tile* tile = grid->get_tile(position);
	if (!grid->is_placeable_tile(tile))
		return;

	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++) {
		if ((*iter_enemy)->is_on_tile(tile)) {
			return;
		}
	}

	Tower* new_tower = new Tower(this, tower_type, tile);
	if (money < new_tower->get_cost_buy())
	{
		delete new_tower;
		return;
	}

	new_tower->set_tile(tile);
	tile->set_tower(new_tower);
	grid->clear_paths();

	if (grid->get_path(grid->get_start_tile(), grid->get_portal_tile())->size() == 0) {
		tile->set_tower(NULL);
		delete new_tower;
		return;
	}

	// loop through all enemies and update their path
	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		if ( !(*iter_enemy)->try_update_path()) {
			// couldn't update path, don't place tower
			tile->set_tower(NULL);
			delete new_tower;
			return;
		}
	}
	//New tower CAN be placed.
	SFX_build->play();
	set_boost_update(true);
	update_enemy_path = true;
	money -= new_tower->get_cost_buy();
	update_money();
	tower_list.push_back(new_tower);
	if (option_box_visible) {
		hide_option_box();
		select(tile);
	}
}

void Game::upgrade_tower(towers::TowerType tower_type)
{
	if (tile_selection != NULL)
	{
		Tower* tower = tile_selection->get_tower();
		if (tower == NULL) return;

		if (money < tower->get_cost_upgrade())
			return;

		int cost = tower->get_cost_upgrade();
		if (!tower->upgrade(tower_type))
			return;
		money -= cost;
		set_boost_update(true);
		tower->set_sell_value(tower->get_sell_value() + cost);
		update_money();
		SFX_upgrade->play();
		if (tower_type > towers::WALL)
			hide_option_box();
		else
			update_option_box();
	}
}

void Game::send_new_wave()
{
	EnemyList tmp = level_control->get_new_wave(this);
	enemy_list.insert(enemy_list.end(), tmp.begin(), tmp.end());
	tmp.clear();
	update_level();
}

bool Game::optbox_do_selection(Sprite* curr_op_sel, GridPosition position)
{
	return optbox_do_selection(curr_op_sel->get_int_type(), position);
}

bool Game::optbox_do_selection(int type, GridPosition position)
{
	/**
	 * Selects an option on the optionbox
	 */
	switch (type)
	{
	case BUTTON_BASE:
	{
		if (grid->get_tile(position)->get_tower() == NULL)
		{
			create_new_tower(towers::SIMPLE, position);
			return true;
		}
		break;
	}
	case BUTTON_BOOST:
	{
		if (grid->get_tile(position)->get_tower() == NULL)
		{
			create_new_tower(towers::BOOST, position);
			return true;
		}
		break;
	}
	case BUTTON_BASIC:
	{
		upgrade_tower(towers::BASIC);
		return true;
	}
	case BUTTON_BOMB:
	{
		upgrade_tower(towers::BOMB);
		return true;
	}
	case BUTTON_RANGE:
	{
		upgrade_tower(towers::RANGE);
		return true;
	}
	case BUTTON_SELL:
	{
		sell(tile_selection);
		return true;
	}
	case BUTTON_SPEED:
	{
		upgrade_tower(towers::SPEED);
		return true;
	}
	case BUTTON_UPGRADE:
	{
		Tower* t = grid->get_tile(position)->get_tower();
		if (t != NULL) {
			upgrade_tower(t->get_type());
			return true;
		} else {
			return false;
		}
		break;
	}
	case BUTTON_WALL:
	{
		create_new_tower(towers::WALL, position);
		return true;
	}
	default:
		break;
	} //Switch
	return false;
}


void Game::arrowkey_bflag_not_set(SDL_Event* event)
{
	int mark_row = 0;
	int mark_col = 0;
	if (tile_selection == NULL) {
		if (last_selected_tile != NULL) {
			mark_row = last_selected_tile->get_position().first;
			mark_col = last_selected_tile->get_position().second;
		}
	} else if (tile_selection != NULL) {
		mark_row = tile_selection->get_position().first;
		mark_col = tile_selection->get_position().second;
	}

	Tile* next_tile = NULL;
	switch (event->key.keysym.sym)
	{
	case SDLK_LEFT:
		next_tile = grid->get_tile(mark_row, mark_col - 1);
		break;
	case SDLK_RIGHT:
		next_tile = grid->get_tile(mark_row, mark_col + 1);
		break;
	case SDLK_UP:
		next_tile = grid->get_tile(mark_row - 1, mark_col);
		break;
	case SDLK_DOWN:
		next_tile = grid->get_tile(mark_row + 1, mark_col);
		break;
	default:
		return;
	}

	if (next_tile != NULL)
		select(next_tile);

}

void Game::buildmenu_selection_not_set(SDL_Event* event)
//Player does not have an object selected from Buildmenu
{
	if (!game_started && event->key.keysym.sym == SDLK_RETURN)
	{
		if (game_state == GAME_PAUSED)
		{
			game_state = GAMEPLAY_RUNNING;
			game_started = true;
			send_new_wave();
			grid->clear_paths();
			for(iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++) {
				(*iter_enemy)->try_update_path();
			}
		}
	}
	if (event->key.keysym.sym == SDLK_SPACE)
	{
		if (selection_sprite->is_visible())
		{
			toggle_option_box();
		}
	}

	update_option_box();
	int position = 0;
	for (iter_op_box = optionbox.begin(); iter_op_box != optionbox.end(); iter_op_box++)
	{
		int posX = (int)selection_sprite->get_x() + 2;
		int posY = (int)selection_sprite->get_y() + 2;
		GridPosition pos = GridPosition(0,0);
		Tile* tile = grid->get_tile_from_mouse(posX, posY);
		if (tile != NULL)
			pos = tile->get_position();

		if (event->key.keysym.sym == SDLK_1 && position == 1)
		{
			if (optbox_do_selection((*iter_op_box), pos))
				break;
		}
		else if (event->key.keysym.sym == SDLK_2 && position == 2)
		{
			if (optbox_do_selection((*iter_op_box), pos))
				break;
		}
		else if (event->key.keysym.sym == SDLK_3 && position == 3)
		{
			if (optbox_do_selection((*iter_op_box), pos))
				break;
		}
		else if (event->key.keysym.sym == SDLK_4 && position == 4)
		{
			if (optbox_do_selection((*iter_op_box), pos))
				break;
		}
		else if (event->key.keysym.sym == SDLK_5 && position == 5)
		{
			if (optbox_do_selection((*iter_op_box), pos))
				break;
		}
		else if (event->key.keysym.sym == SDLK_u)
		{
			if (optbox_do_selection(BUTTON_UPGRADE, pos))
				break;
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			if (optbox_do_selection(BUTTON_SELL, pos))
				break;
		}
		position++;

	}

	if (is_arrow_key(event))
	{
		arrowkey_bflag_not_set(event);
	}
}

void Game::buildmenu_selection_set(SDL_Event* event)
{
	//Player have an object from menu selected
	if (event->key.keysym.sym == SDLK_LEFT)
	{
		iter_build_obj = build_list.end();
		while (build_list.size() > 1)
		{
			iter_build_obj--;
			if (iter_build_obj == build_list.begin())
				break;
			if ((*iter_build_obj) == buildmenu_selection)
			{
				iter_build_obj--;
				select_from_buildmenu((*iter_build_obj));
				break;
			}
		}
	}
	else if (event->key.keysym.sym == SDLK_RIGHT)
	{
		iter_build_obj = build_list.begin();
		while (build_list.size() > 1)
		{
			if ((*iter_build_obj) == buildmenu_selection)
			{
				iter_build_obj++;
				if (iter_build_obj == build_list.end())
					break;
				select_from_buildmenu((*iter_build_obj));
				break;
			}
			iter_build_obj++;
		}

	}

}

void Game::left_mousebutton(int m_x, int m_y)
{
	if (m_x <= GRIDWIDTH && m_y <= GRIDHEIGHT)
	{ //Mouse within Grid
		Tile* tile = grid->get_tile_from_mouse(m_x, m_y);
		if (optionbox_visible())
		{
			bool done = false;
			iter_op_box = optionbox.begin();

			// if mouse is inside OptionBox
			if ((*iter_op_box)->overlaps(m_x, m_y))
			{
				if (tile_selection != NULL)
					tile = tile_selection;
				for (iter_op_box = optionbox.begin()++; iter_op_box != optionbox.end(); iter_op_box++)
				{
					if ((*iter_op_box)->overlaps(m_x, m_y))
					{
						if (tile != NULL)
							done = optbox_do_selection((*iter_op_box), tile->get_position());
					}

					if (done)
						break;
				}
			}

			else //if mouse is outside the box
			{
				if (tile != NULL && tile->get_tower() == NULL)
				{
					cancel_selection();
					hide_option_box();
				}
				else
				{
					//Select Tower on this position
					cancel_selection();
					option_box_visible = false;
					select(tile);
				}
				/*****/

			}
		} //(ShowOptionBox)

		else //if (!ShowOptionBox)

		{
			if (buildmenu_selection != NULL && tile->get_tower() == NULL )
			{
				//Create new tower
				create_new_tower(buildmenu_selection->get_type(), tile->get_position());
			}
			else if (buildmenu_selection == NULL)
			{
				select(tile);
			}
			else // tower is occupying this tile, cannot build there
			{
				SFX_cant_build->play();
			}
		}
	} //Mouse on grid

	else //Mouse on menu

	{
		if (optionbox_visible())
			hide_option_box();
		//Stuff on menu
		for (iter_ingame_button = ingame_buttons.begin(); iter_ingame_button != ingame_buttons.end(); iter_ingame_button++)
		{
			if ((m_x > (*iter_ingame_button)->get_x()) && (m_x < (*iter_ingame_button)->get_x() + (*iter_ingame_button)->get_width()) && (m_y
					> (*iter_ingame_button)->get_y()) && (m_y < (*iter_ingame_button)->get_y() + (*iter_ingame_button)->get_height()))
			{

				if ((*iter_ingame_button)->get_type() == BUTTON_TOGGLEGRID)
				{
					((*iter_ingame_button))->update();
					grid_visible = !grid_visible;
				}

				if ((*iter_ingame_button)->get_type() == BUTTON_TOGGLESOUND)
				{
					((*iter_ingame_button))->update();
					if (Sound::toggle_sound())
						music->play();
					else
						music->stop();
				}

				if ((*iter_ingame_button)->get_type() == BUTTON_MENU) {
					level_control->pause_timer();
					old_game_state = game_state;
					game_state = INGAMEMENU;
				}

				if (tile_selection != NULL &&
						((*iter_ingame_button)->get_type() > BUTTONS) &&
						buildmenu_selection == NULL
						&& !optionbox_visible())
				{
					switch ((*iter_ingame_button)->get_type())
					{
					case BUTTON_UPGR:
						Tower* t = tile_selection->get_tower();
						if (t != NULL)
							upgrade_tower(t->get_type());
						break;
					case BUTTON_SELL:
						sell(tile_selection);
						break;
					}
				}
			}
		}// Iterator-loop ends

		for (iter_build_obj = build_list.begin(); iter_build_obj != build_list.end(); iter_build_obj++)
		{
			if ((m_x > (*iter_build_obj)->get_x()) && (m_x < (*iter_build_obj)->get_x() + 40) && (m_y > (*iter_build_obj)->get_y()) && (m_y
					< (*iter_build_obj)->get_y() + 40))
			{
				select_from_buildmenu((*iter_build_obj));
			}
		}
	}

}

void Game::state_gameplay_running(SDL_Event* event)
{
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	Uint8* keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_f]) FPS_MAX = 1000;
	else FPS_MAX = 50;

	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			cancel_selection();
		}
		else if (game_started && event->key.keysym.sym == SDLK_n)
		{
			send_new_wave();
		}

		if (buildmenu_selection == NULL)
		{
			buildmenu_selection_not_set(event);
		}
		else //Player has selected a tower to build
		{
			buildmenu_selection_set(event);
		}

	}

	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{

		if (event->button.button == SDL_BUTTON_LEFT)
		{
			left_mousebutton(m_x, m_y);
		} //Mousebutton left

		else if (event->button.button == SDL_BUTTON_RIGHT)
		{
			if (buildmenu_selection == NULL && selection_sprite->overlaps(m_x, m_y))
			{
				if (selection_sprite->is_visible())
				{
					toggle_option_box();
				}
			}
			else
			{
				if (option_box_visible)
					hide_option_box();
				else
					cancel_selection();
			}
		}
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		if (tile_selection == NULL && buildmenu_selection == NULL)
		{
			int m_x, m_y;
			SDL_GetMouseState(&m_x, &m_y);

			//Loop through build objects
			for (iter_build_obj = build_list.begin(); iter_build_obj != build_list.end(); iter_build_obj++)
			{
				if ((*iter_build_obj)->overlaps(m_x, m_y))
				{
					hovered_build_item = (*iter_build_obj);
					return;
				}
			}
		}
		hovered_build_item = NULL;
	}

}

void Game::state_introduction(SDL_Event* event)
{
	if (event->key.type == SDL_KEYDOWN)
	{
		game_state = GAME_PAUSED;
	}
}

void Game::state_mainmenu(SDL_Event* event)

{
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	if (event->key.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			for (iter_mainmenu_button = mainmenu_buttons.begin(); iter_mainmenu_button != mainmenu_buttons.end(); iter_mainmenu_button++)
			{
				if ((m_x > (*iter_mainmenu_button)->get_x()) && (m_x < (*iter_mainmenu_button)->get_x() + (*iter_mainmenu_button)->get_width()) && (m_y
						> (*iter_mainmenu_button)->get_y()) && (m_y < (*iter_mainmenu_button)->get_y() + (*iter_mainmenu_button)->get_height()))
				{
					switch ((*iter_mainmenu_button)->get_type())
					{
					case BUTTON_STARTGAME:
						reset_game();
						game_state = GAME_PAUSED;
						break;
					case BUTTON_HIGHSCORE:
						update_highscore_sprites();
						game_state = HIGHSCORE;
						break;
					case BUTTON_VIEW_HELP:
						game_state = VIEW_HELP;
						break;
					case BUTTON_EXITGAME:
						game_running = false;
						break;
					}
				}
			}
		}
	}
}

void Game::state_gameover(SDL_Event* event)
{
	if (event->key.type == SDL_KEYDOWN)
	{
		game_state = MAINMENU;
	}
}

void Game::state_highscore(SDL_Event* event)
{
	if (event->key.type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			error_loading_highscore->hide();
			game_state = MAINMENU;
		}
	}
}

void Game::state_view_help(SDL_Event* event)
{
	if (event->key.type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			game_state = MAINMENU;
		}
	}
}


void Game::state_set_highscore(SDL_Event* event)
{
	if (event->key.type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			//reset_game();
			game_state = MAINMENU;
		}
		else if (event->key.keysym.sym == SDLK_RETURN)
		{
			if (playername != "")
			{
				insert_new_highscore(score, get_highscore_pos(), playername);
				write_highscore_to_file();
				update_highscore_sprites();

				//reset_game();
				game_state = HIGHSCORE;

			}
		}

		// Accept A-Z, a-z, 0-9
		else if (((event->key.keysym.unicode >= (Uint16) 'A') && (event->key.keysym.unicode <= (Uint16) 'Z')) || ((event->key.keysym.unicode >= (Uint16) 'a')
				&& (event->key.keysym.unicode <= (Uint16) 'z')) || ((event->key.keysym.unicode >= (Uint16) '0') && (event->key.keysym.unicode <= (Uint16) '9'))
				|| ((event->key.keysym.unicode == (Uint16) ' ')))
		{
			if (playername.length() < PLAYERNAME_LENGTH_MAX - 1)
			{
				playername += (char) event->key.keysym.unicode;
				input_text->update_text(playername + "_");
			}
			else if (playername.length() == PLAYERNAME_LENGTH_MAX - 1)
			{
				playername += (char) event->key.keysym.unicode;
				input_text->update_text(playername);
			}
		}
		else if ((event->key.keysym.sym == SDLK_BACKSPACE) && (playername.length() != 0))
		{
			playername.erase(playername.length() - 1);
			input_text->update_text(playername + "_");
		}
	}
}

void Game::state_ingame_menu(SDL_Event* event)
{
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	if (event->key.type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			level_control->resume_timer();
			game_state = old_game_state;
		}
	}

	else if (event->key.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			for (iter_ingame_menu_button = ingame_menu_buttons.begin(); iter_ingame_menu_button != ingame_menu_buttons.end(); iter_ingame_menu_button++)
			{
				if ((m_x > (*iter_ingame_menu_button)->get_x())
						&& (m_x < (*iter_ingame_menu_button)->get_x() + (*iter_ingame_menu_button)->get_width()) && (m_y
						> (*iter_ingame_menu_button)->get_y())
						&& (m_y < (*iter_ingame_menu_button)->get_y() + (*iter_ingame_menu_button)->get_height()))
				{
					switch ((*iter_ingame_menu_button)->get_type())
					{
					case BUTTON_RESUMEGAME:
						level_control->resume_timer();
						game_state = old_game_state;
						break;
					case BUTTON_EXITTOMENU:
						//reset_game();
						game_state = MAINMENU;
						break;
					case BUTTON_EXITGAME:
						game_running = false;
						break;
					}
				}
			}
		}
	}
}

void Game::handle_event(SDL_Event* event)
{
	if (event->type == SDL_QUIT)
	{
		game_running = false;
	}

	else if (event->key.type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_F10)
	{
		if (game_state == GAMEPLAY_RUNNING || game_state == GAME_PAUSED)
		{
			level_control->pause_timer();
			old_game_state = game_state;
			game_state = INGAMEMENU;
		}
		else if (game_state == INGAMEMENU) {
			level_control->resume_timer();
			game_state = old_game_state;
		}
	}

	else if (event->key.type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_F11)
		fps_text->is_visible() ? fps_text->hide() : fps_text->show();

	else if (event->key.type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_F12)
		toggle_fullscreen();


	if (game_state == INGAMEMENU)
	{
		state_ingame_menu(event);
	}
	else if (game_state == VIEW_HELP)
	{
		state_view_help(event);
	}
	else if (game_state == MAINMENU)
	{
		state_mainmenu(event);
	}
	else if (game_state == HIGHSCORE)
	{
		state_highscore(event);
	}

	else if (game_state == GAMEOVER)
	{
		state_gameover(event);
	}
	else if (game_state == VIEW_HELP) {
		state_view_help(event);
	}
	else if (game_state == SET_HIGHSCORE)
	{
		state_set_highscore(event);
	}
	else if (game_state == GAMEPLAY_RUNNING || game_state == GAME_PAUSED)
	{
		state_gameplay_running(event);
	}

}
