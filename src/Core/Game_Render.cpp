/*
 * Game_Render.cpp
 *
 *  Created on: Nov 21, 2010
 *      Author: samuel
 */

#include <Core/Game.h>
#include <State/MainMenuState.h>
#include <State/IntroState.h>
#include <State/ViewHelpState.h>
#include <State/HighscoreState.h>
#include <State/GameOverState.h>

void Game::toggle_fullscreen()
{
	if (fullscreen)
	{
		SDL_CreateWindowAndRenderer(WWIDTH, WHEIGHT, 0, &window, &renderer);
		fullscreen = false;
	}
	else
	{
		SDL_CreateWindowAndRenderer(WWIDTH, WHEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
		fullscreen = true;
	}
}
void Game::draw_ingame_menu()
{
	ingame_menu_screen->draw(renderer);
}
void Game::draw_optionbox()
{
	if (!optionbox_visible())
		return;
	for (iter_op_box = optionbox.begin(); iter_op_box != optionbox.end(); iter_op_box++)
	{
		(*iter_op_box)->draw(renderer);
	}
}
void Game::draw_selection()
{
	selection_sprite->draw(renderer);

	BaseTower* t = NULL;
	if (tile_selection != NULL && tile_selection->get_tower() != NULL) {
		t = tile_selection->get_tower();
		t->drawRange(renderer);
	} else if (hovered_build_item != NULL) {
		t = hovered_build_item;
	} else if (buildmenu_selection != NULL) {
		t = buildmenu_selection;
	}

	// draw infosprites
	if (t != NULL) {
		Sprite_List* s = t->get_infosprites();
		for (iter_sel = s->begin(); iter_sel != s->end(); iter_sel++)
		{
			(*iter_sel)->draw(renderer);
		}
	}
}
void Game::draw_enemies()
{
	//Draw Enemies on Grid
	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		(*iter_enemy)->draw(renderer);
	}
	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		(*iter_enemy)->draw_health_bar(renderer);
	}
}
void Game::draw_boost_connections()
{
	//Draw visual connections between boostertowers and other towers
	for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
	{
		if ((*iter_tower)->get_type() == towers::BOOST)

			for (TowerList::iterator iter_tower_2 = tower_list.begin(); iter_tower_2 != tower_list.end(); iter_tower_2++)
			{
				if ((*iter_tower_2)->get_type() != towers::BOOST && (*iter_tower_2)->get_type() != towers::WALL)
				{
					if ((*iter_tower)->get_distance_to((*iter_tower_2)) <= (*iter_tower)->get_range_in_pixels())
					{
						int line_strength = (*iter_tower)->get_level();
						line_strength = 0x33 + line_strength * 12;

						Sint16 x1, x2, y1, y2;
						x1 = (Sint16)((*iter_tower)->get_x() + TILESIZE / 2.0f);
						y1 = (Sint16)((*iter_tower)->get_y() + TILESIZE / 2.0f);
						x2 = (Sint16)((*iter_tower_2)->get_x() + TILESIZE / 2.0f);
						y2 = (Sint16)((*iter_tower_2)->get_y() + TILESIZE / 2.0f);
						lineRGBA(renderer, x1, y1, x2, y2, 0xFF, 0xFF, 0, line_strength);
					}
				}
			}
	}
}
void Game::draw_towers()
{
	//Draw Towers on Grid
	for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
	{
		(*iter_tower)->draw(renderer);
	}
}
void Game::draw_projectiles()
{
	//Draw the exiting projectiles
	for (iter_projectile = projectile_list.begin(); iter_projectile != projectile_list.end(); iter_projectile++)
	{
		(*iter_projectile)->draw(renderer);
	}
}
void Game::draw_menu_towers()
{
	//Draw available towers in menu
	for (iter_build_obj = build_list.begin(); iter_build_obj != build_list.end(); iter_build_obj++)
	{
		(*iter_build_obj)->draw(renderer);
	}
}

void Game::draw_build_item()
{
	// Draw selected builditem and buildability-color.
	if (buildmenu_selection != NULL)
	{
		int m_x, m_y;
		SDL_GetMouseState(&m_x, &m_y);
		int pos_x = m_x;
		int pos_y = m_y;
		Tile* hovered_tile = grid->get_tile_from_mouse(m_x, m_y);

		if (m_x < GRIDWIDTH && hovered_tile != NULL)
		{
			pos_x = hovered_tile->get_x_pixel_pos();
			pos_y = hovered_tile->get_y_pixel_pos();
			if (hovered_tile->get_tower() == NULL)
			{
				//There is no tower, draw green square
				buildmenu_selection->draw(renderer, pos_x, pos_y);
				buildmenu_selection->drawRange(renderer, (float)pos_x, (float)pos_y);
				if (money >= buildmenu_selection->get_cost_buy())
					free_spot->draw(renderer, pos_x, pos_y);
				else
					not_free_spot->draw(renderer, pos_x, pos_y);
			}
			else if (hovered_tile->get_tower() != NULL)
			{
				//There is tower here, draw red square
				//Don't draw tower if there is tower of same type
				if (buildmenu_selection->get_type() != hovered_tile->get_tower()->get_type())
				{
					buildmenu_selection->draw(renderer, pos_x, pos_y);
				}
				buildmenu_selection->drawRange(renderer, (float)pos_x, (float)pos_y);
				not_free_spot->draw(renderer, pos_x, pos_y);
			}
		}
		else
		{
			buildmenu_selection->draw(renderer, m_x - 20, m_y - 20);
		}
	}
}
void Game::draw_ingame_buttons()
{
	//Draw Buttons
	for (iter_ingame_button = ingame_buttons.begin(); iter_ingame_button != ingame_buttons.end(); iter_ingame_button++)
	{
		(*iter_ingame_button)->draw(renderer);
	}
}
void Game::draw_ingame_menu_buttons()
{
	//Draw Buttons
	for (iter_ingame_menu_button = ingame_menu_buttons.begin(); iter_ingame_menu_button != ingame_menu_buttons.end(); iter_ingame_menu_button++)
	{
		(*iter_ingame_menu_button)->draw(renderer);
	}
}
void Game::draw_money_score()
{
	money_text->draw(renderer);
	split_money_score->draw(renderer);
	score_text->draw(renderer);
	lives_text->draw(renderer);
	level_text->draw(renderer);
	timer_text->draw(renderer);
}
void Game::render_gameplay()
{
	map->draw(renderer);
	if (grid_visible)
		map_grid->draw(renderer);

	map_wall->draw(renderer);
	map_entrance->draw(renderer);
	map_exit->draw(renderer);

	draw_boost_connections();
	draw_towers();

	menu_background->draw(renderer);
	menu_money_score->draw(renderer);
	menu_build->draw(renderer);
	menu_upgrade->draw(renderer);
	menu_info->draw(renderer);
	menu_lives->draw(renderer);

	draw_money_score();
	draw_enemies();
	speed_text->draw(renderer);
	draw_selection();
	map_exit->draw(renderer);
	draw_projectiles();
	draw_menu_towers();
	draw_ingame_buttons();
	draw_optionbox();
	draw_build_item();

	if (!game_started) press_enter_to_start->draw(renderer);

}

void Game::render()
{
	SDL_RenderClear(renderer);

	if (game_state == DEVSCREEN || game_state == INTROSCREEN)
	{
		introState->render(renderer);
	}

	else if (game_state == MAINMENU)
	{
		mainMenuState->render(renderer);
	}

	else if (game_state == VIEW_HELP)
	{
		viewHelpState->render(renderer);
	}
	
	else if (game_state == HIGHSCORE || game_state == SET_HIGHSCORE)
	{
		highscoreState->render(renderer);
	}

	else if (game_state == INGAMEMENU)
	{
		render_gameplay();
		draw_ingame_menu();
		draw_ingame_menu_buttons();
	}

	else if (game_state == GAMEPLAY_RUNNING || game_state == GAME_PAUSED)
	{
		render_gameplay();
	}

	else if (game_state == GAMEOVER)
	{
		render_gameplay();
		gameOverState->render(renderer);
	}

	fps_text->draw(renderer);

	SDL_RenderPresent(renderer);
}
