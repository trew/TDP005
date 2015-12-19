/*
 * Sound.h
 *
 *  Created on: Nov 28, 2010
 *      Author: bjorn
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <SDL_mixer.h>
#include <iostream>


/** 
 * Enables sound and music.
 *
 * The sound class enables sound and music. You can repeat sounds and effects.
 */
class Sound
{
public:
	Sound(const std::string filename, bool is_music_in, int repeat_in); ///< Constructor
	virtual ~Sound();		///< Destructor
	void play();			///< Play sound
	void stop();			///< Stop playing sound

	static void setSoundEnabled(bool enabled);
	static bool isSoundEnabled();

	static int setVolume(int volume); ///< Sets the volume and returns the new volume. 0-128
	static int getVolume();		   ///< Returns the current volume. 0-128

private:
	bool isMusic { false };
	int repeat { -1 };
	Mix_Music *music { nullptr };
	Mix_Chunk *effect { nullptr };

	int channel { -1 };

	static bool soundEnabled;
};

#endif /* SOUND_H_ */
