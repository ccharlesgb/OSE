#pragma once

#include "../Engine/BasePhysics.h"
#include <iostream>

class Player : public BasePhysics
{
private:
	BaseObject *mWeapon;
public:
	Player(void);
	~Player(void);
	void Spawn();
	void Think();
};

