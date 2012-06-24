#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/EntityList.h"

#define USE_DELAY 0.5f

class Player : public BasePhysics
{
private:
	EntityList<BaseObject*> mWeapons;
	BaseObject *mActiveWeapon;
	float mNextUse;
public:
	Player(void);
	~Player(void);
	void Spawn();
	void Think();

	void GiveWeapon(BaseObject *wep);
	void ChooseWeapon(const char* name);
	void PhysicsSimulate(float delta);
};

