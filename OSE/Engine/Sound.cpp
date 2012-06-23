#include "Sound.h"
#include "Resource.h"

Sound::Sound(const char* path)
{
	mSound = *Resource::RequestSound(path);
	mHeight = 0;
	mSound.setMinDistance(10);
	mSound.setAttenuation(0.1f);
}

Sound::~Sound(void)
{
	
}

/**
 * Play the sound.
 */
void Sound::Play()
{
	mSound.play();
}

/**
 * Pause the sound.
 */
void Sound::Pause()
{
	mSound.pause();
}

/**
 * Stop the sound.
 */
void Sound::Stop()
{
	mSound.stop();
}

/**
 * Set if the sound should loop.
 */
void Sound::SetLoop(bool loop)
{
	mSound.setLoop(loop);
}

/**
 * Set the sound origion.
 */
void Sound::SetPosition(Vector2 position)
{
	position = ig::GameToSFML(position);
	mSound.setPosition(position.x, position.y, (float)mHeight);
}

/**
 * Return the position, converted to our cord system.
 */
Vector2 Sound::GetPosition()
{
	sf::Vector3f pos = mSound.getPosition();
	return ig::SFMLToGame(*new Vector2(pos.x, pos.y));
}

/**
 * Set if the sound should be relative to player or absolute.
 */
void Sound::SetRelativeToListener(bool relative)
{
	mSound.setRelativeToListener(relative);
}

/**
 * Set the distance before the sound starts "fading" out.
 */
void Sound::SetMinDistance(float distance)
{
	mSound.setMinDistance(distance);
}

/**
 * Set the Attenuation (rate it fades).
 *
 * Range between 0 and 100. 0 means it dosn't fade.
 */
void Sound::SetAttenuation(float attenuation)
{
	mSound.setAttenuation(attenuation);
}