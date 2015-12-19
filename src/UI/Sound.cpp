/*
 * Sound.cpp
 *
 *  Created on: Nov 28, 2010
 *      Author: bjorn
 */

#include <UI/Sound.h>
#include <Utils/Log.h>

bool Sound::soundEnabled = true;

bool Sound::isSoundEnabled()
{
	return soundEnabled;
}

void Sound::setSoundEnabled(bool enabled)
{
	soundEnabled = enabled;
}

int Sound::setVolume(int vol)
{
	if (vol < 0) vol = 0;
	if (vol > 128) vol = 128;

	setSoundEnabled(vol != 0);

	Mix_VolumeMusic(vol);
	Mix_Volume(-1, vol);

	return vol;
}

int Sound::getVolume()
{
	return Mix_Volume(-1, -1);
}

Sound::Sound(std::string _file, bool _isMusic, int _repeat)
	: isMusic(_isMusic), repeat(_repeat), music(NULL), effect(NULL)
{
	if (isMusic)
	{
		music = Mix_LoadMUS(_file.c_str());
		if (music == NULL)
		{
			LOG_ERROR << "Sound " << _file.c_str() << "cound not be loaded.";
			return;
		}
	}
	else
	{
		effect = Mix_LoadWAV(_file.c_str());

		if (effect == NULL)
		{
			LOG_ERROR << "Sound " << _file.c_str() << " could not be loaded.";
			return;
		}
	}
}

Sound::~Sound()
{
	if (isMusic)
	{
		Mix_FreeMusic(music);
	}
	else
	{
		Mix_FreeChunk(effect);
	}
}

void Sound::play()
{
	if (soundEnabled)
	{
		if (isMusic && music != NULL)
		{
			Mix_PlayMusic(music, repeat);
		}
		else if (!isMusic && effect != NULL)
		{
			Mix_PlayChannel(-1, effect, repeat);
		}
	}
}

void Sound::stop()
{
	if (isMusic && music != NULL)
	{
		Mix_HaltMusic();
	}
}
