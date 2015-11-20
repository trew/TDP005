/*
 * Projectile.cpp
 *
 *  Created on: Nov 23, 2010
 *      Author: bjorn
 */

#include <Entity/Projectile.h>
#include <Core/Game.h>
#include <Core/Define.h>
#include <cmath>
#include <string>
#include <iostream>

Projectile::Projectile(Game* game, std::string file, float x_pos_in, float y_pos_in, double direction_in , double move_speed_in, int damage_in, double splash_area_in, int time)
:Sprite(game), movespeed(move_speed_in), direction(direction_in), damage(damage_in), splash_area(splash_area_in)
{
	dead = false;
	if(splash_area>0) {
		type = PROJECTILE_BOMB;
		life_time = time;
	}
	else {
		type = PROJECTILE;
		life_time = time;
	}
	x_pos = x_pos_in;
	y_pos = y_pos_in;
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


void Projectile::update(int delta)
{
	life_time -= delta;
	if (life_time < 0) {
		dead = true;
		return;
	}
	double radianAngle = conv_degree_to_radian(direction);
	x_pos += (movespeed * cos(radianAngle)) * get_game()->get_time_modifier();
	y_pos += (movespeed * sin(radianAngle)) * get_game()->get_time_modifier();
}

void Projectile::draw(SDL_Surface* dest_surf) {
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = (Sint16)x_pos;
	dest_rect.y = (Sint16)y_pos;
	SDL_BlitSurface(sprite_surf, NULL, dest_surf, &dest_rect);
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
