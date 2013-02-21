/*
 * Tower_Specifics.h
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#ifndef TOWER_SPECIFICS_H_
#define TOWER_SPECIFICS_H_

int round_double(double x)
{
	///Returns a rounded double

	double a;
	if (modf(x, &a) >= 0.5)
		return (x >= 0) ? (int) ceil(x) : (int) floor(x);
	else
		return (x < 0) ? ceil(x) : floor(x);
}

int format_range(double range)
{
	///Formats the range

	if (range == 0)
		return range;
	return round_double((range * TILESIZE) + 20);
}

int tower_base_damage = 10;
int tower_base_range = format_range(1.0);
int tower_base_upgradecost = 20;

// BASIC TOWER SETTINGS //////////////////
// Range
int tower_basic_1_range = format_range(1.3);
int tower_basic_2_range = format_range(2.0);
int tower_basic_3_range = format_range(2.3);
// Damage
int tower_basic_1_damage = 20;
int tower_basic_2_damage = 30;
int tower_basic_3_damage = 70;
// ReloadingTime
int tower_basic_1_reloadingtime = 20;
int tower_basic_2_reloadingtime = 18;
int tower_basic_3_reloadingtime = 14;
// RotationSpeed
double tower_basic_1_rotationspeed = 7.0;
double tower_basic_2_rotationspeed = 8.0;
double tower_basic_3_rotationspeed = 10.0;
// ProjectileSpeed
double tower_basic_1_projectilespeed = 12.0;
double tower_basic_2_projectilespeed = 15.0;
double tower_basic_3_projectilespeed = 20.0;
// Upgrade cost
int tower_basic_1_upgradecost = 40;
int tower_basic_2_upgradecost = 200;

// SPEED TOWER SETTINGS //////////////////
// Range
int tower_speed_1_range = format_range(1.2);
int tower_speed_2_range = format_range(1.7);
int tower_speed_3_range = format_range(2);
// Damage
int tower_speed_1_damage = 16;
int tower_speed_2_damage = 18;
int tower_speed_3_damage = 30;
// ReloadingTime
int tower_speed_1_reloadingtime = 14;
int tower_speed_2_reloadingtime = 10;
int tower_speed_3_reloadingtime = 5;
// RotationSpeed
double tower_speed_1_rotationspeed = 8.0;
double tower_speed_2_rotationspeed = 10.0;
double tower_speed_3_rotationspeed = 12.0;
// ProjectileSpeed
double tower_speed_1_projectilespeed = 10.0;
double tower_speed_2_projectilespeed = 12.0;
double tower_speed_3_projectilespeed = 14.0;
// Upgrade cost
int tower_speed_1_upgradecost = 40;
int tower_speed_2_upgradecost = 200;

// RANGE TOWER SETTINGS //////////////////
// Range
int tower_range_1_range = format_range(2.2);
int tower_range_2_range = format_range(2.8);
int tower_range_3_range = format_range(3.7);
// Damage
int tower_range_1_damage = 35;
int tower_range_2_damage = 45;
int tower_range_3_damage = 130;
// ReloadingTime
int tower_range_1_reloadingtime = 40;
int tower_range_2_reloadingtime = 37;
int tower_range_3_reloadingtime = 35;
// RotationSpeed
double tower_range_1_rotationspeed = 5.0;
double tower_range_2_rotationspeed = 5.5;
double tower_range_3_rotationspeed = 6.0;
// ProjectileSpeed
double tower_range_1_projectilespeed = 15.0;
double tower_range_2_projectilespeed = 17.0;
double tower_range_3_projectilespeed = 19.0;
// Upgrade cost
int tower_range_1_upgradecost = 40;
int tower_range_2_upgradecost = 200;

// BOMB TOWER SETTINGS ///////////////////
// Range
int tower_bomb_1_range = format_range(1.2);
int tower_bomb_2_range = format_range(1.8);
int tower_bomb_3_range = format_range(2.2);
// Damage
int tower_bomb_1_damage = 15;
int tower_bomb_2_damage = 20;
int tower_bomb_3_damage = 35;
// ReloadingTime
int tower_bomb_1_reloadingtime = 100;
int tower_bomb_2_reloadingtime = 90;
int tower_bomb_3_reloadingtime = 80;
// RotationSpeed
double tower_bomb_1_rotationspeed = 6.0;
double tower_bomb_2_rotationspeed = 7.0;
double tower_bomb_3_rotationspeed = 8.0;
// ProjectileSpeed
double tower_bomb_1_projectilespeed = 4.0;
double tower_bomb_2_projectilespeed = 5.0;
double tower_bomb_3_projectilespeed = 6.0;
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
int tower_boost_1_range = format_range(1.2);
int tower_boost_2_range = format_range(1.5);
int tower_boost_3_range = format_range(2);
// Boost-modifier (saved in parameter "damage")
int tower_boost_1_modifier = 20;
int tower_boost_2_modifier = 25;
int tower_boost_3_modifier = 35;
// Upgrade cost
int tower_boost_1_upgradecost = 40;
int tower_boost_2_upgradecost = 200;

#endif /* TOWER_SPECIFICS_H_ */
