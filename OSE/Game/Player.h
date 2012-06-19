#pragma once

#include "BasePhysics.h"
#include <iostream>

class Player : public BasePhysics
{
private:

public:
	Player(void);
	~Player(void);
	void Spawn();
	void Think();
	void DefinePhysics();
};

