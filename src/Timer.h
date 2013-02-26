/*
 * Timer.h
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <SDL/SDL.h>

class Timer {
public:
	Timer();
	virtual ~Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 get_ticks();
	bool is_started();
	bool is_paused();

private:
	Uint32 startTicks;
	Uint32 pausedTicks;

	bool paused;
	bool started;


};

#endif /* TIMER_H_ */
