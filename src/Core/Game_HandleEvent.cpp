/*
 * Game_HandleEvent.cpp
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
#include <State/InGameMenuState.h>
#include <State/GamePlayState.h>
#include <Utils/Utils.h>
#include <string>

void Game::createNewTower(const towers::TowerType &tower_type, const GridPosition &position, Grid &grid)
{
	Tile* tile = grid.get_tile(position);
	if (!grid.is_placeable_tile(tile))
		return;

	for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++) {
		if ((*iter_enemy)->is_on_tile(tile)) {
			return;
		}
	}

	BaseTower* new_tower = new BaseTower(this, tower_type, tile);
	if (money < new_tower->get_cost_buy())
	{
		delete new_tower;
		new_tower = NULL;
		return;
	}
	new_tower->set_tile(tile);
	tile->set_tower(new_tower);
	grid.clear_paths();

	Tile* startTile = grid.get_start_tile();
	Tile* portalTile = grid.get_portal_tile();

	Path* path = grid.get_path(startTile, portalTile);

	if (path->size() == 0) {
		tile->set_tower(NULL);
		grid.clear_paths();
		Path* new_path = grid.get_path(grid.get_start_tile(), grid.get_portal_tile());
		delete new_path; new_path = NULL;
		delete new_tower; new_tower = NULL;
		delete path; path = NULL;
		return;
	}

	// loop through all enemies and update their path
	for (EnemyList::iterator iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
	{
		if ( !(*iter_enemy)->try_update_path()) {
			// couldn't update path, don't place tower
			tile->set_tower(NULL);
			grid.clear_paths();
			Path* new_path = grid.get_path(grid.get_start_tile(), grid.get_portal_tile());
			delete new_path; new_path = NULL;
			delete new_tower; new_tower = NULL;
			delete path; path = NULL;
			return;
		}
	}

	delete path; path = NULL;

	//New tower CAN be placed.
	SFX_build->play();
	set_boost_update(true);
	update_enemy_path = true;
	money -= new_tower->get_cost_buy();
	tower_list.push_back(new_tower);
	if (gamePlayState->isOptionBoxVisible())
	{
		gamePlayState->setOptionBoxVisible(false);
		gamePlayState->select(tile);
	}
}

bool Game::sell(Tile* tile)
{
	if (tile == NULL)
		return false;
	BaseTower* tower = tile->get_tower();

	//Tries to sell empty spot
	if (tower == NULL)
		return false;
	else
	{
		if (gamePlayState->getSubState() == GAME_PAUSED)
			money += tower->get_sell_value();
		else
			money += tower->get_sell_value() / 2;

		SFX_sell->play();
		set_boost_update(true);
		update_enemy_path = true;
		tower->get_tile()->set_tower(NULL);
		tower_list.remove(tower);
		delete tower;
		tower = NULL;

		grid->clear_paths();
		for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
		{
			(*iter_enemy)->try_update_path();
		}
		gamePlayState->updateOptionBox();
	}
	return true;
}

void Game::upgrade_tower(BaseTower* const tower, towers::TowerType tower_type)
{
	if (tower == NULL) return;

	int cost = tower->get_cost_upgrade();
	if (money < cost)
	{
		return;
	}

	if (!tower->upgrade(tower_type))
	{
		return;
	}
	money -= cost;
	set_boost_update(true);
	SFX_upgrade->play();
	if (tower_type > towers::WALL)
	{
		gamePlayState->setOptionBoxVisible(false);
	}
	else
	{
		gamePlayState->updateOptionBox();
	}
}

void Game::upgrade_tower(BaseTower* const tower)
{
	towers::TowerType tower_type = tower->get_type();
	upgrade_tower(tower, tower_type);
}
