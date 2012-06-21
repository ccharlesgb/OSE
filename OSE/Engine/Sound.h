#pragma once

#include <SFML/Audio.hpp>
#include "Vector2.h"

class Sound
{
private:
	int mHeight = 0;
	sf::Sound mSound;
public:
	Sound(const char* path);
	~Sound(void);
	
	void Play();
	void SetPosition(Vector2 position);
};

