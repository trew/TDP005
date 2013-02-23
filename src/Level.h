/*
 * EnemyWave.h
 *
 *  Created on: 1 dec 2010
 *      Author: samuel
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <list>
#include "Enemy.h"
#include <ctime>
#include <cstdlib>

/** \brief Controls difficulty and waves.
 *
 * The level class manages enemy waves and difficulty of the game. It uses a pool of virtual money to "buy" enemies.
 * This pool rises with each level.
 * Every 5 level a boss appears and the level after that is allowed to use a new enemy type.
 * After 20 levels, there is no new types.
 */
class Level {
public:
	Level();		///< Constructor
	virtual ~Level(); ///< Destructor

	EnemyList get_new_wave(Grid* grid);		///< Returns a new wave depending on current level

	int get_level();				///< Get current level

	bool time_to_send_wave();		///< Time to send next wave?
	bool last_enemy_is_sent();		///< Is last enemy on grid?
	void set_last_enemy_sent();		///< Last enemy is on grid
	void reset();					///< Reset level to 1

	int time_before_next_wave();	///< Returns time in seconds
private:
	void compose_new_wave(Grid* grid);

	EnemyList enemies_in_wave;
	int current_level;
	int current_boss_level;

	bool last_enemy_sent;
	Uint32 wave_delay;
	Uint32 wave_start_delay;

};

#endif /* LEVEL_H_ */
