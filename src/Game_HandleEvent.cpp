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

void Game::create_new_tower(int tower_type, int pos_x, int pos_y)
{
	if (pos_x == 0 && pos_y == 7 * TILESIZE)
		return;

	Tower* new_tower = new Tower(tower_type, pos_x, pos_y);
	if (money < new_tower->get_cost_buy())
	{
		delete new_tower;
		return;
	}
	grid_control[get_grid_position(pos_x, pos_y)] = dev_screen;
	if (path_control->find_paths(grid_control, 105, ENEMY_DESTINATION).empty())
	{
		grid_control[get_grid_position(pos_x, pos_y)] = NULL;
		return;
	}


	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		if ((((*iter_enemy)->get_current_destination().first == pos_x) && ((*iter_enemy)->get_current_destination().second == pos_y))
				|| ((!(*iter_enemy)->can_update_path())))
		{
			if ( (*iter_enemy)->get_current_destination() != std::pair<int, int>(15 * TILESIZE, 7 * TILESIZE) ) //Enemy has reached goal, enable tower placement
			{
				grid_control[get_grid_position(pos_x, pos_y)] = NULL;
				return;
			}
		}
	}

	//New tower CAN be placed.
	SFX_build->play();
	update_enemy_path = true;
	money -= new_tower->get_cost_buy();
	update_money();
	tower_list.push_back(new_tower);
	grid_control[get_grid_position(pos_x, pos_y)] = tower_list.back();
	if (!building_flag)
	{
		current_selection = grid_control[get_grid_position(pos_x, pos_y)];
		select(current_selection);
		hide_option_box();
	}

}

void Game::upgrade_tower(int tower_type)
{
	if (current_selection != NULL)
	{
		if (money < current_selection->get_cost_upgrade())
			return;

		int cost = current_selection->get_cost_upgrade();
		if (!current_selection->upgrade(tower_type))
			return;
		money -= cost;
		current_selection->add_to_sell_value(cost);
		update_money();
		SFX_upgrade->play();
		set_selection_info(current_selection);
		if (tower_type > 0)
			hide_option_box();
		else
			update_option_box();
	}
}

void Game::send_new_wave()
{
	Sprite_List tmp = level_control->get_new_wave();
	enemy_list.insert(enemy_list.end(), tmp.begin(), tmp.end());
	tmp.clear();
	update_level();
}

bool Game::optbox_do_selection(Sprite* curr_op_sel, int pos_x, int pos_y)
{
	return optbox_do_selection(curr_op_sel->get_type(), pos_x, pos_y);
}

bool Game::optbox_do_selection(int type, int pos_x, int pos_y)
{
	switch (type)
	{
	case BUTTON_BASE:
		if (grid_control[get_grid_position(pos_x, pos_y)] == NULL)
		{
			create_new_tower(TOWER_BASE, pos_x, pos_y);
		}
		else
		{
			upgrade_tower(TOWER_BASIC_LEVEL_1);
		}
		return true;

	case BUTTON_BOOST:
		create_new_tower(TOWER_BOOST_LEVEL_1, pos_x, pos_y);
		return true;

	case BUTTON_BASIC:
		upgrade_tower(TOWER_BASIC_LEVEL_1);

		return true;

	case BUTTON_BOMB:
		upgrade_tower(TOWER_BOMB_LEVEL_1);

		return true;
	case BUTTON_RANGE:
		upgrade_tower(TOWER_RANGE_LEVEL_1);

		return true;
	case BUTTON_SELL:
		sell(current_selection);

		return true;
	case BUTTON_SPEED:
		upgrade_tower(TOWER_SPEED_LEVEL_1);

		return true;
	case BUTTON_UPGRADE:
		upgrade_tower(0);

		return true;
	case BUTTON_WALL:
		create_new_tower(TOWER_WALL, pos_x, pos_y);
		return true;
	default:
		break;
	} //Switch
	return false;
}

void Game::arrowkey_bflag_not_set(SDL_Event* event)
{
	int mark_x = selection_sprite->get_x_pos();
	int mark_y = selection_sprite->get_y_pos();

	if (current_selection != NULL)
	{
		mark_x = current_selection->get_x_pos();
		mark_y = current_selection->get_y_pos();

		switch (event->key.keysym.sym)
		{
		case SDLK_LEFT:

			if (grid_control[get_grid_position(mark_x - TILESIZE, mark_y)] != NULL)
			{
				cancel_selection();
				select(grid_control[get_grid_position(mark_x - TILESIZE, mark_y)]);
			}
			else
			{
				if (mark_x - TILESIZE > -5)
				{
					cancel_selection();
					selection_sprite->show();
					selection_sprite->set_x_pos(mark_x - TILESIZE - 2);
					selection_sprite->set_y_pos(mark_y - 2);
				}
			}
			break;
		case SDLK_RIGHT:

			if (grid_control[get_grid_position(mark_x + TILESIZE, mark_y)] != NULL)
			{
				cancel_selection();
				select(grid_control[get_grid_position(mark_x + TILESIZE, mark_y)]);
			}
			else
			{
				if (mark_x + TILESIZE <= GRIDWIDTH - TILESIZE)
				{
					cancel_selection();
					selection_sprite->show();
					selection_sprite->set_x_pos(mark_x + TILESIZE - 2);
					selection_sprite->set_y_pos(mark_y - 2);
				}
			}
			break;
		case SDLK_UP:

			if (grid_control[get_grid_position(mark_x, mark_y - TILESIZE)] != NULL)
			{
				cancel_selection();
				select(grid_control[get_grid_position(mark_x, mark_y - TILESIZE)]);
			}
			else
			{
				if (mark_y - TILESIZE > -5)
				{
					cancel_selection();
					selection_sprite->show();
					selection_sprite->set_x_pos(mark_x - 2);
					selection_sprite->set_y_pos(mark_y - TILESIZE - 2);
				}
			}
			break;
		case SDLK_DOWN:

			if (grid_control[get_grid_position(mark_x, mark_y + TILESIZE)] != NULL)
			{
				cancel_selection();
				select(grid_control[get_grid_position(mark_x, mark_y + TILESIZE)]);
			}
			else
			{
				if (mark_y + TILESIZE < GRIDHEIGHT)
				{
					cancel_selection();
					selection_sprite->show();
					selection_sprite->set_x_pos(mark_x - 2);
					selection_sprite->set_y_pos(mark_y + TILESIZE - 2);
				}
			}
			break;
		default:
			break;
		}
	}
	else if (current_selection == NULL)
	{
		if (mark_x > -5 && mark_x < GRIDWIDTH) //Fixed bug: selection_sprite stuck on right side of grid when deselecting with ESC. You could move it around very buggy then.
		{
			switch (event->key.keysym.sym)
			{
			case SDLK_LEFT:

				if (mark_x - TILESIZE > -3)
				{
					if (grid_control[get_grid_position(mark_x + 2 - TILESIZE, mark_y + 2)] != NULL)
					{
						//Select
						hide_option_box();
						select(grid_control[get_grid_position(mark_x + 2 - TILESIZE, mark_y + 2)]);
					}
					else
					{
						cancel_selection();
						selection_sprite->show();
						selection_sprite->set_x_pos(mark_x - TILESIZE);
						selection_sprite->set_y_pos(mark_y);
					}
				}
				break;
			case SDLK_RIGHT:

				if (mark_x + TILESIZE < GRIDWIDTH - TILESIZE)
				{
					if (grid_control[get_grid_position(mark_x + 2 + TILESIZE, mark_y + 2)] != NULL)
					{
						//Select
						hide_option_box();
						select(grid_control[get_grid_position(mark_x + 2 + TILESIZE, mark_y + 2)]);
					}
					else
					{
						cancel_selection();
						selection_sprite->show();
						selection_sprite->set_x_pos(mark_x + TILESIZE);
						selection_sprite->set_y_pos(mark_y);
					}
				}
				break;
			case SDLK_UP:

				if (mark_y - TILESIZE > -3)
				{
					if (grid_control[get_grid_position(mark_x + 2, mark_y + 2 - TILESIZE)] != NULL)
					{
						//Select
						hide_option_box();
						select(grid_control[get_grid_position(mark_x + 2, mark_y + 2 - TILESIZE)]);
					}
					else
					{
						cancel_selection();
						selection_sprite->show();
						selection_sprite->set_x_pos(mark_x);
						selection_sprite->set_y_pos(mark_y - TILESIZE);
					}
				}
				break;
			case SDLK_DOWN:

				if (mark_y + TILESIZE < GRIDHEIGHT - TILESIZE)
				{
					if (grid_control[get_grid_position(mark_x + 2, mark_y + 2 + TILESIZE)] != NULL)
					{
						//Select
						hide_option_box();
						select(grid_control[get_grid_position(mark_x + 2, mark_y + 2 + TILESIZE)]);
					}
					else
					{
						cancel_selection();
						selection_sprite->show();
						selection_sprite->set_x_pos(mark_x);
						selection_sprite->set_y_pos(mark_y + TILESIZE);
					}
				}
				break;
			default:
				break;
			}
		}
		else
		{
			update_option_box();
			selection_sprite->show();
			selection_sprite->set_x_pos(-2);
			selection_sprite->set_y_pos(-2);
		}
	}
}

void Game::buildingflag_not_set(SDL_Event* event)
//Player does not have an object selected from Buildmenu
{
	if (!game_started && event->key.keysym.sym == SDLK_RETURN)
	{
		if (game_state == GAME_PAUSED)
		{
			game_state = GAMEPLAY_RUNNING;
			game_started = true;
			send_new_wave();
			for(iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++) {
				(*iter_enemy)->can_update_path();
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
		int posX = selection_sprite->get_x_pos() + 2;
		int posY = selection_sprite->get_y_pos() + 2;

		if (event->key.keysym.sym == SDLK_1 && position == 1)
		{
			if (optbox_do_selection((*iter_op_box), posX, posY))
				break;
		}
		else if (event->key.keysym.sym == SDLK_2 && position == 2)
		{
			if (optbox_do_selection((*iter_op_box), posX, posY))
				break;
		}
		else if (event->key.keysym.sym == SDLK_3 && position == 3)
		{
			if (optbox_do_selection((*iter_op_box), posX, posY))
				break;
		}
		else if (event->key.keysym.sym == SDLK_4 && position == 4)
		{
			if (optbox_do_selection((*iter_op_box), posX, posY))
				break;
		}
		else if (event->key.keysym.sym == SDLK_5 && position == 5)
		{
			if (optbox_do_selection((*iter_op_box), posX, posY))
				break;
		}
		else if (event->key.keysym.sym == SDLK_u)
		{
			if (optbox_do_selection(BUTTON_UPGRADE, posX, posY))
				break;
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			if (optbox_do_selection(BUTTON_SELL, posX, posY))
				break;
		}
		position++;

	}

	if (is_arrow_key(event))
	{
		arrowkey_bflag_not_set(event);
	}
}

void Game::buildingflag_set(SDL_Event* event)
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
			if ((*iter_build_obj) == current_selection)
			{
				iter_build_obj--;
				select((*iter_build_obj));
				break;
			}
		}
	}
	else if (event->key.keysym.sym == SDLK_RIGHT)
	{
		iter_build_obj = build_list.begin();
		while (build_list.size() > 1)
		{
			if ((*iter_build_obj) == current_selection)
			{
				iter_build_obj++;
				if (iter_build_obj == build_list.end())
					break;
				select((*iter_build_obj));
				break;
			}
			iter_build_obj++;
		}

	}

}

void Game::left_mousebutton(int m_x, int m_y, SDL_Event* event)
{
	if (m_x <= GRIDWIDTH && m_y <= GRIDHEIGHT)
	{ //Mouse within Grid
		if (optionbox_visible())
		{
			bool done = false;
			int pos_x = selection_sprite->get_x_pos() + 2;
			int pos_y = selection_sprite->get_y_pos() + 2;
			iter_op_box = optionbox.begin();
			if ((m_x > (*iter_op_box)->get_x_pos()) && (m_x < (*iter_op_box)->get_x_pos() + (*iter_op_box)->get_width()) && (m_y > (*iter_op_box)->get_y_pos())
					&& (m_y < (*iter_op_box)->get_y_pos() + (*iter_op_box)->get_height()))
			{
				for (iter_op_box = optionbox.begin()++; iter_op_box != optionbox.end(); iter_op_box++)
				{

					if ((m_x > (*iter_op_box)->get_x_pos()) && (m_x < (*iter_op_box)->get_x_pos() + (*iter_op_box)->get_width()) && (m_y
							> (*iter_op_box)->get_y_pos()) && (m_y < (*iter_op_box)->get_y_pos() + (*iter_op_box)->get_height()))
					{
						done = optbox_do_selection((*iter_op_box), pos_x, pos_y);
					} //if mouseoverlap

					if (done)
						break;
				} //for loop
			} // if Mouse inside OptionBox

			else //if mouse IS outside the box
			{
				/*****/
				//Snap mouseX and mouseY to Grid

				pos_x = m_x;
				pos_y = m_y;
				snap_XY_to_grid(pos_x, pos_y);

				if (grid_control[get_grid_position(pos_x, pos_y)] == NULL)
				{
					cancel_selection();
					hide_option_box();
					selection_sprite->show();
					selection_sprite->set_x_pos(pos_x - 2);
					selection_sprite->set_y_pos(pos_y - 2);
				}
				else
				{
					//Select Tower on this position
					cancel_selection();
					option_box_visible = false;
					selection_sprite->set_x_pos(pos_x - 2);
					selection_sprite->set_y_pos(pos_y - 2);
					select(grid_control[get_grid_position(pos_x, pos_y)]);
				}
				/*****/

			}
		} //(ShowOptionBox)

		else //if (!ShowOptionBox)

		{
			//Snap mouseX and mouseY to Grid

			int pos_x = m_x;
			int pos_y = m_y;
			snap_XY_to_grid(pos_x, pos_y);

			if (grid_control[get_grid_position(pos_x, pos_y)] == NULL)
			{
				//Create new tower
				if (current_selection != NULL && building_flag == true)
				{
					create_new_tower(current_selection->get_type(), pos_x, pos_y);
				}
				else
				{
					cancel_selection();
					option_box_visible = false;
					selection_sprite->show();
					selection_sprite->set_x_pos(pos_x - 2);
					selection_sprite->set_y_pos(pos_y - 2);
				}

			}
			else if (building_flag == false)
			{
				//Select Tower on this position
				selection_sprite->show();
				selection_sprite->set_x_pos(pos_x - 2);
				selection_sprite->set_y_pos(pos_y - 2);
				select(grid_control[get_grid_position(pos_x, pos_y)]);
			}
			else
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
			if ((m_x > (*iter_ingame_button)->get_x_pos()) && (m_x < (*iter_ingame_button)->get_x_pos() + (*iter_ingame_button)->get_width()) && (m_y
					> (*iter_ingame_button)->get_y_pos()) && (m_y < (*iter_ingame_button)->get_y_pos() + (*iter_ingame_button)->get_height()))
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
					old_game_state = game_state;
					game_state = INGAMEMENU;
				}

				if (current_selection != NULL && current_selection->get_type() < ENEMY && ((*iter_ingame_button)->get_type() > BUTTONS) && !building_flag
						&& !optionbox_visible())
				{

					switch ((*iter_ingame_button)->get_type())
					{

					case BUTTON_UPGR:
						upgrade_tower(0);
						break;

					case BUTTON_SELL:
						sell(current_selection);
						break;
					}
				}
			}
		}// Iterator-loop ends

		for (iter_build_obj = build_list.begin(); iter_build_obj != build_list.end(); iter_build_obj++)
		{
			if ((m_x > (*iter_build_obj)->get_x_pos()) && (m_x < (*iter_build_obj)->get_x_pos() + 40) && (m_y > (*iter_build_obj)->get_y_pos()) && (m_y
					< (*iter_build_obj)->get_y_pos() + 40))
			{
				select((*iter_build_obj));
				building_flag = true;
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
	else FPS_MAX = 100;

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

		if (!building_flag)
		{
			buildingflag_not_set(event);
		}
		else //Player has selected a tower to build
		{
			buildingflag_set(event);
		}

	}

	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{

		if (event->button.button == SDL_BUTTON_LEFT)
		{
			left_mousebutton(m_x, m_y, event);
		} //Mousebutton left

		else if (event->button.button == SDL_BUTTON_RIGHT)
		{
			if (!building_flag && (m_x > selection_sprite->get_x_pos()) && (m_x < selection_sprite->get_x_pos() + TILESIZE) && (m_y
					> selection_sprite->get_y_pos()) && (m_y < selection_sprite->get_y_pos() + TILESIZE))
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
		if (current_selection == NULL)
		{
			int m_x, m_y;
			SDL_GetMouseState(&m_x, &m_y);
			bool hovering = false;

			//Loop through build objects
			for (iter_build_obj = build_list.begin(); iter_build_obj != build_list.end(); iter_build_obj++)
			{
				if ((m_x > (*iter_build_obj)->get_x_pos()) && (m_x < (*iter_build_obj)->get_x_pos() + 40) && (m_y > (*iter_build_obj)->get_y_pos()) && (m_y
						< (*iter_build_obj)->get_y_pos() + 40))
				{
					set_selection_info((*iter_build_obj));
					hovering = true;
				}
			}
			if (hovering == false)
			{
				clear_selectioninfo();
			}
		}
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
				if ((m_x > (*iter_mainmenu_button)->get_x_pos()) && (m_x < (*iter_mainmenu_button)->get_x_pos() + (*iter_mainmenu_button)->get_width()) && (m_y
						> (*iter_mainmenu_button)->get_y_pos()) && (m_y < (*iter_mainmenu_button)->get_y_pos() + (*iter_mainmenu_button)->get_height()))
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
			game_state = old_game_state;
		}
	}

	else if (event->key.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			for (iter_ingame_menu_button = ingame_menu_buttons.begin(); iter_ingame_menu_button != ingame_menu_buttons.end(); iter_ingame_menu_button++)
			{
				if ((m_x > (*iter_ingame_menu_button)->get_x_pos())
						&& (m_x < (*iter_ingame_menu_button)->get_x_pos() + (*iter_ingame_menu_button)->get_width()) && (m_y
						> (*iter_ingame_menu_button)->get_y_pos())
						&& (m_y < (*iter_ingame_menu_button)->get_y_pos() + (*iter_ingame_menu_button)->get_height()))
				{
					switch ((*iter_ingame_menu_button)->get_type())
					{
					case BUTTON_RESUMEGAME:
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
			old_game_state = game_state;
			game_state = INGAMEMENU;
		}
		else if (game_state == INGAMEMENU)
			game_state = old_game_state;
	}

	else if (event->key.type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_F11)
		if (fps_text->is_visible()) fps_text->hide();
		else fps_text->show();

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
