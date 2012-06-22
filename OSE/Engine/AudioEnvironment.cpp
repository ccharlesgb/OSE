#include "AudioEnvironment.h"

BaseObject* sAudioEnvironment::mListener = NULL;

/**
 * Set the Audio Listener to the current listeners postion.
 */
void sAudioEnvironment::Update()
{
	if (mListener == NULL) return;
	
	Vector2 position = ig::GameToSFML(mListener->GetPos());
	Vector2 angle = ig::AngleToVector(mListener->GetAngle());
	sf::Listener::setPosition(position.x, position.y, 0);
	sf::Listener::setDirection(angle.x, angle.y, 0);
}