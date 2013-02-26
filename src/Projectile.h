/*
 * Projectile.h
 *
 *  Created on: Nov 23, 2010
 *      Author: bjorn
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "Sprite.h"

/** \brief Projectile class
 *
 * The projectile X- and Y-positions are a bit tricky.
 * While using integers for blitting to surface and such, doubles are used for the actual position.
 * If not, angles wouldn't be as specific and quite buggy actually.
 */
class Projectile: public Sprite
{
public:
	///Creates a projectile using a lot of different parameters.
	Projectile(Game* game, std::string file, float x_pos_in, float y_pos_in, double direction_in , double movespeed_in, int damage_in, double splash_area_in, int life_time);
	virtual ~Projectile();	///< Destructor

	void draw(SDL_Surface* dest_surf);		///< Draw projectile to screen
	void update();			///< Update projectile

	int get_damage();		///< Get damage from projectile
	double get_splash_area();	///< Should be used to give damage to enemies in an area. NOT DONE!
	bool is_dead();				///< Is projectile dead? (out of screen, hit an enemy?)

private:
	double movespeed;
	double direction;
	int damage;
	double splash_area;

private:
	Uint32 life_time;
	bool dead;
	double conv_radian_to_degree(double a);
    double conv_degree_to_radian(double a);
};

#endif /* PROJECTILE_H_ */


