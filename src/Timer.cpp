/*
 * Timer.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include "Timer.h"

Timer::Timer() {
	startTicks = pausedTicks = 0;
	started = paused = false;
}

Timer::~Timer() {
}

void Timer::start() {
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}

void Timer::stop() {
	startTicks = pausedTicks = 0;
	started = false;
	paused = false;
}

void Timer::pause() {
	if (started && !paused) {
		paused = true;
		pausedTicks = SDL_GetTicks();
	}
}

void Timer::unpause() {
	if (paused && started) {
		startTicks += SDL_GetTicks() - pausedTicks;
		paused = false;
	}
}

Uint32 Timer::get_ticks() {
	if (is_started()) {
		if (is_paused()) {
			return pausedTicks;
		} else {
			return SDL_GetTicks() - startTicks;
		}
	}
	return 0;
}

bool Timer::is_started() {
	return started;
}

bool Timer::is_paused() {
	return paused;
}
