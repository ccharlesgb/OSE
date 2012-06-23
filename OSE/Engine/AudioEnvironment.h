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
};

