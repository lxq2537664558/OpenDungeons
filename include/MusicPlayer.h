#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

//#include <OgreOggSoundManager.h>
//#include <OgreOggISound.h>
#include <SFML/Audio.hpp>
#include <OgreResourceGroupManager.h>
#include <vector>
#include <iostream>

/*! \brief Class to manage playing of music.
 *
 */
class MusicPlayer { // : public OgreOggSound::OgreOggISound::SoundListener {
public:
	MusicPlayer();
	//virtual ~MusicPlayer();
	void load();
	void start();
	void stop();
	void next();

private:
	void startCurrent();
	//void soundStopped(OgreOggSound::OgreOggISound* sound);

	std::vector<sf::Music> tracks;
	bool loaded;
	unsigned currentTrack;
};

#endif /* MUSICPLAYER_H */
