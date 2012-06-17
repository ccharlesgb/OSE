#pragma once

#include "BasePhysics.h"
#include <iostream>

class Player : public BasePhysics
{
private:
	float Speed;
	float TankSpeed;
public:
	static void Stop(BaseObject* ent, VariantMap &Data);
	Player(void);
	~Player(void);
	void Spawn();
	void Think();
	void DefinePhysics();
};

