#include "Sound.h"
#include "Resource.h"

Sound::Sound(const char* path)
{
	mSound = *Resource::RequestSound(path);
}

Sound::~Sound(void)
{
	
}

void Sound::Play()
{
	mSound.play();
}

void Sound::SetPosition(Vector2 position)
{
	position = ig::GameToSFML(position);
	mSound.setPosition(position.x, position.y, mHeight);
}

