/*
 * Define.h
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#ifndef DEFINE_H_
#define DEFINE_H_

const int STARTING_MONEY = 100;
const int STARTING_LIVES = 20;

const int DEVSCREEN 	=	1;
const int INTROSCREEN 	=	2;
const int MAINMENU		=  	3;
const int HIGHSCORE		=	4;
const int SET_HIGHSCORE	=	5;
const int VIEW_HELP		=	6;
const int GAMEPLAY_RUNNING = 7;
const int GAME_PAUSED 	=	8;
const int INGAMEMENU 	=	9;
const int GAMEOVER		=	10;
const int EXIT 			=	0;


const int MENUWIDTH 		=	220;
const int GRIDHEIGHT		=	600;
const int GRIDWIDTH		=	600;
const int WWIDTH 			=	800;
const int WHEIGHT 			=	600;
const int WBPP 				=	32;
const int TILESIZE 		=	40;


const int PLAYERNAME_LENGTH_MAX = 10;


const int BUTTON_STARTGAME	= 	1;
const int BUTTON_HIGHSCORE	=	2;
const int BUTTON_VIEW_HELP  =   3;
const int BUTTON_EXITGAME	=	4;
const int BUTTON_EXITTOMENU	=	5;
const int BUTTON_RESUMEGAME	=	6;

const int BUTTONS			=	100;
const int BUTTON_MENU		=	101;
const int BUTTON_UPGR 		=	102;
const int BUTTON_BASE		=	103;
const int BUTTON_BASIC		=	104;
const int BUTTON_BOMB		=	105;
const int BUTTON_BOOST		=	106;
const int BUTTON_RANGE		=	107;
const int BUTTON_SELL		=	108;
const int BUTTON_SPEED		=	109;
const int BUTTON_UPGRADE	=	110;
const int BUTTON_NOUPGRADE	=	111;
const int BUTTON_WALL		=	112;
const int BUTTON_TOGGLESOUND=	113;
const int BUTTON_TOGGLEGRID	=	114;

const int PROJECTILE		=	25;
const int PROJECTILE_BOMB	=	26;

const int ENEMY_DESTINATION	=	119;

enum Direction {
	NONE,
	RIGHT,
	LEFT,
	UP,
	DOWN
};

#endif /* DEFINE_H_ */