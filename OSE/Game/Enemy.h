#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/EntityList.h"
#include "../Engine/Text.h"

class Enemy : public BasePhysics
{
private:
	BaseObject* mTarget;
public:
	Enemy(void);
	~Enemy(void);
	void Spawn();
	void Think();
	void Draw();

	void PhysicsSimulate(float delta);
};

