#pragma once

#include "BaseHuman.h"
#include "../Engine/EntityList.h"
#include "../Engine/Text.h"
#include "../Game/weapon_pistol.h"

#define USE_DELAY 0.5f

class Player : public BaseHuman
{
private:
	EntityList<BaseObject*> mWeapons;
	weapon_pistol *mActiveWeapon;
	float mNextUse;

	BaseObject* mLight;

	float mLastTakeDamage;
public:
	Player(void);
	~Player(void);

	void Spawn();
	void Think();

	void GiveWeapon(BaseObject *wep);
	void ChooseWeapon(const char* name);

	void PhysicsSimulate(float delta);

	void TakeDamage(const DamageInfo &info);

	BaseObject* GetActiveWeapon() {return mActiveWeapon;};
};

