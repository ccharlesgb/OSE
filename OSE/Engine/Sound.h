#pragma once

#include <SFML/Audio.hpp>
#include "Utilities/Vector2.h"

class Sound
{
private:
	int mHeight;
	sf::Sound mSound;
public:
	
	enum Status {
		Stopped,
		Paused,
		Playing
	};
	
	Sound(const char* path);
	~Sound(void);
	
	void Play();
	void Pause();
	void Stop();
	
	Status GetStatus() { return (Status) mSound.getStatus(); };
	
	void SetLoop(bool loop);
	bool GetLoop() { return mSound.getLoop(); };
	
	void SetVolume(float volume) {mSound.setVolume(volume);};
	bool GetVolume() { return mSound.getVolume(); };
	
	void SetPitch(float pitch) {mSound.setPitch(pitch);};
	bool GetPitch() { return mSound.getPitch(); };
	
	void SetPosition(Vector2 position);
	Vector2 GetPosition();
	
	void SetRelativeToListener(bool relative);
	bool IsRelativeToListener() { return mSound.isRelativeToListener(); };
	
	void SetMinDistance(float distance);
	float GetMinDistance() { return mSound.getMinDistance(); };
	
	void SetAttenuation(float attenuation);
	float GetAttenuation() { return mSound.getAttenuation(); };
};

