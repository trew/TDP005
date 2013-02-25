/*
 * Tower_Specifics.h
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#ifndef TOWER_SPECIFICS_H_
#define TOWER_SPECIFICS_H_

float format_range(float range)
{
	///Formats the range

	if (range == 0.0)
		return 0.0f;
	return (range * (float)TILESIZE) + 20.0f;
}

int tower_base_damage = 10;
float tower_base_range = format_range(1.0f);
int tower_base_upgradecost = 20;

// BASIC TOWER SETTINGS //////////////////
// Range
float tower_basic_1_range = format_range(1.3f);
float tower_basic_2_range = format_range(2.0f);
float tower_basic_3_range = format_range(2.3f);
// Damage
int tower_basic_1_damage = 20;
int tower_basic_2_damage = 30;
int tower_basic_3_damage = 70;
// ReloadingTime
float tower_basic_1_reloadingtime = 20;
float tower_basic_2_reloadingtime = 18;
float tower_basic_3_reloadingtime = 14;
// RotationSpeed
float tower_basic_1_rotationspeed = 7.0f;
float tower_basic_2_rotationspeed = 8.0f;
float tower_basic_3_rotationspeed = 10.0f;
// ProjectileSpeed
float tower_basic_1_projectilespeed = 12.0f;
float tower_basic_2_projectilespeed = 15.0f;
float tower_basic_3_projectilespeed = 20.0f;
// Upgrade cost
int tower_basic_1_upgradecost = 40;
int tower_basic_2_upgradecost = 200;

// SPEED TOWER SETTINGS //////////////////
// Range
float tower_speed_1_range = format_range(1.2f);
float tower_speed_2_range = format_range(1.7f);
float tower_speed_3_range = format_range(2.0f);
// Damage
int tower_speed_1_damage = 16;
int tower_speed_2_damage = 18;
int tower_speed_3_damage = 30;
// ReloadingTime
float tower_speed_1_reloadingtime = 14.0f;
float tower_speed_2_reloadingtime = 10.0f;
float tower_speed_3_reloadingtime = 5.0f;
// RotationSpeed
float tower_speed_1_rotationspeed = 8.0f;
float tower_speed_2_rotationspeed = 10.0f;
float tower_speed_3_rotationspeed = 12.0f;
// ProjectileSpeed
float tower_speed_1_projectilespeed = 10.0f;
float tower_speed_2_projectilespeed = 12.0f;
float tower_speed_3_projectilespeed = 14.0f;
// Upgrade cost
int tower_speed_1_upgradecost = 40;
int tower_speed_2_upgradecost = 200;

// RANGE TOWER SETTINGS //////////////////
// Range
float tower_range_1_range = format_range(2.2f);
float tower_range_2_range = format_range(2.8f);
float tower_range_3_range = format_range(3.7f);
// Damage
int tower_range_1_damage = 35;
int tower_range_2_damage = 45;
int tower_range_3_damage = 130;
// ReloadingTime
float tower_range_1_reloadingtime = 40.0f;
float tower_range_2_reloadingtime = 37.0f;
float tower_range_3_reloadingtime = 35.0f;
// RotationSpeed
float tower_range_1_rotationspeed = 5.0f;
float tower_range_2_rotationspeed = 5.5f;
float tower_range_3_rotationspeed = 6.0f;
// ProjectileSpeed
float tower_range_1_projectilespeed = 15.0f;
float tower_range_2_projectilespeed = 17.0f;
float tower_range_3_projectilespeed = 19.0f;
// Upgrade cost
int tower_range_1_upgradecost = 40;
int tower_range_2_upgradecost = 200;

// BOMB TOWER SETTINGS ///////////////////
// Range
float tower_bomb_1_range = format_range(1.2f);
float tower_bomb_2_range = format_range(1.8f);
float tower_bomb_3_range = format_range(2.2f);
// Damage
int tower_bomb_1_damage = 15;
int tower_bomb_2_damage = 20;
int tower_bomb_3_damage = 35;
// ReloadingTime
float tower_bomb_1_reloadingtime = 100.0f;
float tower_bomb_2_reloadingtime = 90.0f;
float tower_bomb_3_reloadingtime = 80.0f;
// RotationSpeed
float tower_bomb_1_rotationspeed = 6.0f;
float tower_bomb_2_rotationspeed = 7.0f;
float tower_bomb_3_rotationspeed = 8.0f;
// ProjectileSpeed
float tower_bomb_1_projectilespeed = 4.0f;
float tower_bomb_2_projectilespeed = 5.0f;
float tower_bomb_3_projectilespeed = 6.0f;
// Upgrade cost
int tower_bomb_1_upgradecost = 40;
int tower_bomb_2_upgradecost = 200;
int tower_bomb_3_upgradecost = 0;

// BOOST TOWER SETTINGS ///////////////////
//-----------------------------------------
// Settings for Boost tower Level 1 can be found in
// Tower::Tower(int new_type, int x, int y)
//-----------------------------------------
// Range
float tower_boost_1_range = format_range(1.2f);
float tower_boost_2_range = format_range(1.5f);
float tower_boost_3_range = format_range(2.0f);
// Boost-modifier (saved in parameter "damage")
float tower_boost_1_modifier = 0.2f;
float tower_boost_2_modifier = 0.25f;
float tower_boost_3_modifier = 0.35f;
// Upgrade cost
int tower_boost_1_upgradecost = 40;
int tower_boost_2_upgradecost = 200;

#endif /* TOWER_SPECIFICS_H_ */
