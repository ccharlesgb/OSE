#include "Sound.h"
#include "Resource.h"

Sound::Sound(const char* path)
{
	mSound = *Resource::RequestSound(path);
	mHeight = 0;
	mSound.setMinDistance(10);
	mSound.setAttenuation(0.1);
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

void Sound::SetMinDistance(float distance)
{
	mSound.setMinDistance(distance);
}

void Sound::SetAttenuation(float attenuation)
{
	mSound.setAttenuation(attenuation);
}