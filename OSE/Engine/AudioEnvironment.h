#pragma once

#include <SFML/Audio.hpp>
#include "Utilities/Vector2.h"
#include "Bases/BaseObject.h"

class sAudioEnvironment
{
private:
	static BaseObject* mListener;
public:
	static void SetListener(BaseObject* listener) { mListener = listener; };
	static void Update();
	static void SetMasterVolume(float vol) {sf::Listener::setGlobalVolume(vol);};
	static float GetMasterVolume() {return sf::Listener::getGlobalVolume();};
};

