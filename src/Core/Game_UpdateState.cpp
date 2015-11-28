/*
 * Game_UpdateState.cpp
 *
 *  Created on: 29 nov 2010
 *      Author: samuel
 */

#include <Core/Game.h>
#include <State/MainMenuState.h>
#include <State/IntroState.h>
#include <State/HighscoreState.h>
#include <State/GameOverState.h>
#include <State/InGameMenuState.h>
#include <State/GamePlayState.h>
#include <Utils/Utils.h>
#include <iostream>
#include <Core/GameEngine.h>

void Game::update_boost() {
	if (need_boost_update) {
		for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++) {
			(*iter_tower)->update_boost();
		}

		need_boost_update = false;
	}
}

void Game::get_rewards(Enemy* enemy) {
	score += enemy->get_reward_score();
	money += enemy->get_reward_money();
}

void Game::update(const float &timeStep)
{
	if (gamePlayState->getSubState() != GAMEPLAY_RUNNING) {
		update_boost();
		return;
	}

	iter_enemy = enemy_list.begin();
	while(iter_enemy != enemy_list.end())
	{
		if(update_enemy_path)
		{
			(*iter_enemy)->update_path();
		}

		(*iter_enemy)->update(timeStep);

		if ((*iter_enemy)->is_killed())						//Enemy was killed
		{
			get_rewards((*iter_enemy));

			for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
			{
				if((*iter_tower)->get_target() == (*iter_enemy))
					(*iter_tower)->set_target(NULL);
			}

			if((*iter_enemy) != NULL)
			{
				delete (*iter_enemy);
			}
			(*iter_enemy) = NULL;

			iter_enemy = enemy_list.erase(iter_enemy);
		}
		else if ((*iter_enemy)->has_reached_goal())
		{
			//Enemy reached end
			//Punish Player
			lives -= 1;
			if(lives >= 1)
			{
				SFX_life_lost->play();
			}
			else
			{
				gameOverState->setScore(score);
				highscoreState->setScore(score);

				if (highscoreState->isQualifyingScore(score))
				{
					SFX_new_highscore->play();
					getEngine()->setState(highscoreState);
					highscoreState->setInternalState(ENTER);
				}
				else
				{
					SFX_game_over->play();
					getEngine()->setState(gameOverState);
				}
			}

			for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
			{
				if ((*iter_tower)->get_target() == (*iter_enemy))
				{
					(*iter_tower)->set_target(NULL);
				}
			}

			delete (*iter_enemy);
			(*iter_enemy) = NULL;

			iter_enemy = enemy_list.erase(iter_enemy);
		}
		else
		{
			iter_enemy++;
		}
	}

	update_enemy_path = false;

	update_boost();

	for (iter_tower = tower_list.begin(); iter_tower != tower_list.end(); iter_tower++)
	{
		(*iter_tower)->update(timeStep);
	}

	ProjectileList new_projectiles;
	iter_projectile = projectile_list.begin();
	while (iter_projectile != projectile_list.end())
	{
		bool projectileIteratorUpdated = false;
		(*iter_projectile)->update(timeStep);

		float p_x, p_y; // Projectile X and Y position
		int p_w, p_h;
		p_x = (*iter_projectile)->get_x();
		p_y = (*iter_projectile)->get_y();
		p_w = (*iter_projectile)->get_width();
		p_h = (*iter_projectile)->get_height();

		///Remove projectile if it has hit a target or if its position is out of the grid
		if ((p_x > GRIDWIDTH + p_w || p_x < 0 - p_w) ||
			(p_y > GRIDHEIGHT + p_h || p_y < 0 - p_h) ||
			(*iter_projectile)->is_dead())
		{
			delete (*iter_projectile);
			(*iter_projectile) = NULL;
			iter_projectile = projectile_list.erase(iter_projectile);
			continue;
		}

		float e_x, e_y; // Enemy X and Y position
		int e_w, e_h; // Enemy Width and Height

		// Check if projectile has collided with an enemy
		for (iter_enemy = enemy_list.begin(); iter_enemy != enemy_list.end(); iter_enemy++)
		{
			if (enemy_list.empty())
				break;
			e_x = (*iter_enemy)->get_x();
			e_y = (*iter_enemy)->get_y();
			e_h = (*iter_enemy)->get_height();
			e_w = (*iter_enemy)->get_width();

			///Collision detection
			if ((*iter_enemy)->intersects((*iter_projectile)))
			{
				(*iter_enemy)->take_damage((*iter_projectile)->get_damage());

				//Spawn a lot of projectiles around enemy
				if ((*iter_projectile)->get_int_type() == PROJECTILE_BOMB) {
					for (int i = 0; i < 90; i += 15) {

						int new_projectile_dmg = (int)((*iter_projectile)->get_damage() / 2.7f);
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x, e_y - 1, 225 + i, 50, new_projectile_dmg, 0, 600));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x + e_w / 4, e_y - 1, 247 + i, 100, new_projectile_dmg, 0, 420));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x + e_w / 2, e_y - 1, 270 + i, 50, new_projectile_dmg, 0, 630));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x + 3 * (e_w / 4), e_y - 1, 292 + i, 100, new_projectile_dmg, 0, 400));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x + e_w, e_y - 1, 315 + i, 50, new_projectile_dmg, 0, 600));

						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x + (e_w)+1, e_y + (e_h / 4), 337 + i, 100, new_projectile_dmg, 0, 420));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x + (e_w)+1, e_y + (e_h / 2), 0 + i, 50, new_projectile_dmg, 0, 630));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x + (e_w)+1, e_y + 3 * (e_h / 4), 22 + i, 100, new_projectile_dmg, 0, 400));

						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x + e_w, e_y + e_h, 45 + i, 50, new_projectile_dmg, 0, 600));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x + 3 * (e_w / 4), e_y + e_h, 67 + i, 100, new_projectile_dmg, 0, 420));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x + (e_w / 2), e_y + e_h, 90 + i, 50, new_projectile_dmg, 0, 630));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x + e_w / 4, e_y + e_h, 112 + i, 100, new_projectile_dmg, 0, 400));

						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x - 1, e_y + e_h, 135 + i, 50, new_projectile_dmg, 0, 600));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x - 1, e_y + 3 * (e_h / 4), 157 + i, 100, new_projectile_dmg, 0, 420));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-bomb.png", e_x - 1, e_y + (e_h / 2), 180 + i, 50, new_projectile_dmg, 0, 630));
						new_projectiles.push_back(new Projectile(this, "./gfx/tower/ammo/ammo-speed.png", e_x - 1, e_y + e_h / 4, 202 + i, 100, new_projectile_dmg, 0, 400));
					}
				}

				delete (*iter_projectile);
				(*iter_projectile) = NULL;
				iter_projectile = projectile_list.erase(iter_projectile);
				projectileIteratorUpdated = true; // indicates that the iterator should not be incremented at the end
				break;
			}
		}

		if (!projectileIteratorUpdated)
		{
			iter_projectile++;
		}
	}

	projectile_list.insert(projectile_list.end(), new_projectiles.begin(), new_projectiles.end());
	new_projectiles.clear();

	//Check if its time to send new wave
	if (enemy_list.empty() ||
			(enemy_list.back()->get_x() >= 0 && !level_control->last_enemy_is_sent()) ) {
		level_control->set_last_enemy_sent();
	}
	if (level_control->time_to_send_wave(timeStep))
	{
		sendNewWave();
	}
}
