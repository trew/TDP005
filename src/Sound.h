/*
 * Sound.h
 *
 *  Created on: Nov 28, 2010
 *      Author: bjorn
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <SDL/SDL_mixer.h>
#include <iostream>


/** \brief Enables sound and music.
 *
 * The sound class enables sound and music. You can repeat sounds and effects.
 * Sound can also be muted and unmuted using toggle_sound().
 */
class Sound
{
public:
	Sound(std::string filename, bool is_music_in, int repeat_in); ///< Constructor
	virtual ~Sound();		///< Destructor
	void play();			///< Play sound
	void stop();			///< Stop playing sound
	static bool toggle_sound();		///< Mute/Unmute

	static int set_volume(int volume); ///< Sets the volume and returns the new volume. 0-128
	static int get_volume();		   ///< Returns the current volume. 0-128
	static bool enabled();

private:
	bool is_music;
	int repeat;
	Mix_Music *my_music;
	Mix_Chunk *my_effect;
	static bool play_sounds;
};

#endif /* SOUND_H_ */
