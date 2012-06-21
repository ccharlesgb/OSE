#pragma once

#include "../Engine/BasePhysics.h"
#include <iostream>

#define USE_DELAY 0.5f

class Player : public BasePhysics
{
private:
	BaseObject *mWeapon;
	float mNextUse;
public:
	Player(void);
	~Player(void);
	void Spawn();
	void Think();
};

