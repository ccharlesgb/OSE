#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/EntityList.h"
#include "../Engine/Text.h"
#include "../Game/weapon_pistol.h"

#define USE_DELAY 0.5f

class Player : public BasePhysics
{
private:
	EntityList<BaseObject*> mWeapons;
	weapon_pistol *mActiveWeapon;
	float mNextUse;
	Text *mText;
public:
	Player(void);
	~Player(void);
	void Spawn();
	void Think();
	void Draw();

	void GiveWeapon(BaseObject *wep);
	void ChooseWeapon(const char* name);
	void PhysicsSimulate(float delta);
};

