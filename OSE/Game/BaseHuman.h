#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/EntityList.h"
#include "../Engine/Text.h"
#include "../Game/weapon_pistol.h"

#define USE_DELAY 0.5f

class BaseHuman : public BasePhysics
{
private:
	float mWalkSpeed;
	float mHeadAngle;
	bool mInVehicle;
	BaseObject *mVehicle;
protected:
	Sprite* mHead;
	Sprite* mFeet;
public:
	BaseHuman(void);
	~BaseHuman(void);
	void Spawn();
	void Draw();

	void SetHeadAngle(float angle) {mHeadAngle = angle;};
	float GetHeadAngle() {return mHeadAngle;};

	void CreateHead(const char *texture_path);
	void CreateFeet(const char *texture_path);

	void DrawHead();
	void DrawFeet();

	virtual void PhysicsSimulate(float delta);

	void SetWalkSpeed(float speed) {mWalkSpeed = speed;};
	float GetWalkSpeed() {return mWalkSpeed;};

	void EnterVehicle(BaseObject *ent);
	void ExitVehicle(BaseObject* ent);

	bool InVehicle() {return mInVehicle;};
};

