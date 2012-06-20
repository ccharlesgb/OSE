#pragma once

#include "../Engine/BasePhysics.h"
#include <iostream>

class Ship : public BasePhysics
{
private:
	bool InUse;
public:
	Ship(void);
	~Ship(void);
	void Spawn();
	void OnDelete();
	void Think();
	void StartTouch(CollisionInfo* info);
};
