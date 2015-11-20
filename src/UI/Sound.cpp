/*
 * Sound.cpp
 *
 *  Created on: Nov 28, 2010
 *      Author: bjorn
 */

#include <UI/Sound.h>
bool Sound::play_sounds = true;

Sound::Sound(std::string file_in, bool is_music_in, int repeat_in) :
	is_music(is_music_in), repeat(repeat_in), my_music(NULL), my_effect(NULL)
{
	if (is_music)
	{
		my_music = Mix_LoadMUS(file_in.c_str());
		if (my_music == NULL)
		{
			std::cerr << "Sound " << file_in.c_str() << " could not be loaded." << std::endl;
			return;
		}
	}

	else
	{
		my_effect = Mix_LoadWAV(file_in.c_str());

		if (my_effect == NULL)
		{
			std::cerr << "Sound " << file_in.c_str() << " could not be loaded." << std::endl;
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
		Mix_HaltMusic();
	}
}

bool Sound::toggle_sound()
{
	play_sounds = !play_sounds;
	return play_sounds;
}

bool Sound::enabled() {
	return play_sounds;
}

int Sound::set_volume(int vol) {
	if (vol < 0 || vol > 128)
		return Mix_Volume(-1, -1);
	play_sounds = vol != 0;
	if (vol > 15)
		Mix_VolumeMusic(vol - 15);
	else if (vol > 0)
		Mix_VolumeMusic(1);
	else
		Mix_VolumeMusic(0);
	return Mix_Volume(-1, vol);
}

int Sound::get_volume() {
	return Mix_Volume(-1, -1);
}
