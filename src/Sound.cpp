/*
 * Sound.cpp
 *
 *  Created on: Nov 28, 2010
 *      Author: bjorn
 */

#include "Sound.h"
bool Sound::play_sounds = true;

Sound::Sound(std::string file_in, bool is_music_in, int repeat_in) :
	is_music(is_music_in), repeat(repeat_in), my_music(NULL), my_effect(NULL)
{
	if (is_music)
	{
		my_music = Mix_LoadMUS(file_in.c_str());
		if (my_music == NULL)
		{
			std::cerr << "Sound " << file_in << " could not be loaded." << std::endl;
			return;
		}
	}

	else
	{
		my_effect = Mix_LoadWAV(file_in.c_str());

		if (my_effect == NULL)
		{
			std::cerr << "Sound " << file_in << " could not be loaded." << std::endl;
			return;
		}
	}
}

Sound::~Sound()
{
	if (is_music)
		Mix_FreeMusic(my_music);
	else
		Mix_FreeChunk(my_effect);
}

void Sound::play()
{
	if (play_sounds)
	{
		if (is_music && my_music != NULL)
			Mix_PlayMusic(my_music, repeat);
		else if (!is_music && my_effect != NULL)
			Mix_PlayChannel(-1, my_effect, repeat);
	}
}

void Sound::stop()
{
	if (is_music && my_music != NULL) {
		Mix_FadeOutMusic(500);
	}
}

bool Sound::toggle_sound()
{
	if (play_sounds)
	{
		play_sounds = false;
		return false;
	}
	else
	{
		play_sounds = true;
		return true;
	}
}
