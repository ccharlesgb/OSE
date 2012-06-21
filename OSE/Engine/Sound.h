#pragma once

#include <SFML/Audio.hpp>
#include "Utilities/Vector2.h"

class Sound
{
private:
	int mHeight;
	sf::Sound mSound;
public:
	Sound(const char* path);
	~Sound(void);
	
	void Play();
	void SetPosition(Vector2 position);
	void SetMinDistance(float distance);
	void SetAttenuation(float attenuation);
};

