/*
 * Projectile.cpp
 *
 *  Created on: Nov 23, 2010
 *      Author: bjorn
 */

#include "Projectile.h"
#include "Define.h"
#include <cmath>
#include <string>
#include <iostream>

Projectile::Projectile(std::string file, int x_pos_in, int y_pos_in, double direction_in , double move_speed_in, int damage_in, double splash_area_in, int time)
:movespeed(move_speed_in), direction(direction_in), damage(damage_in), splash_area(splash_area_in)
{
	dead = false;
	if(splash_area>0) {
		type = PROJECTILE_BOMB;
		life_time = SDL_GetTicks() +time;
	}
	else {
		type = PROJECTILE;
		life_time = SDL_GetTicks() +time;
	}
	x_pos = x_pos_in;
	y_pos = y_pos_in;
	real_x_pos = x_pos_in;
	real_y_pos = y_pos_in;
	sprite_surf = load_image(file.c_str());
}

Projectile::~Projectile() {
}

double Projectile::conv_radian_to_degree(double a)
{
	return (a * 57.295779513082);
}

double Projectile::conv_degree_to_radian(double a)
{
	return (a * 0.017453292519);
}


void Projectile::update()
{
	if(life_time < SDL_GetTicks()) dead = true;
	double radianAngle = conv_degree_to_radian(direction);
	real_x_pos = real_x_pos + movespeed * cos(radianAngle);
	real_y_pos = real_y_pos + movespeed * sin(radianAngle);
	x_pos = (int)real_x_pos;
	y_pos = (int)real_y_pos;
}

void Projectile::draw(SDL_Surface* dest_surf) {
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = x_pos;
	dest_rect.y = y_pos;
	SDL_BlitSurface(sprite_surf, NULL, dest_surf, &dest_rect);
}

int Projectile::get_x_pos() {
	return (int)real_x_pos;
}
int Projectile::get_y_pos() {
	return (int)real_y_pos;
}

bool Projectile::is_dead() {
	return dead;
}

int Projectile::get_damage() {
	return damage;
}
double Projectile::get_splash_area() {
	///Not finished!
	return splash_area;
}
