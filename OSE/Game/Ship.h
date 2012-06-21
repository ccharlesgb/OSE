#pragma once

#include "../Engine/BasePhysics.h"
#include <iostream>

class Ship : public BasePhysics
{
private:
	BaseObject* mDriver;
	bool InUse() { return mDriver != NULL; };
	float mWheelAngle;
	float mThrottle;
	float mEnterTime;
public:
	Ship(void);
	~Ship(void);
	void Spawn();
	void OnDelete();
	void Think();
	void StartTouch(CollisionInfo* info);
	void Exit(Vector2 position);
	void Use(BaseObject *ply);
};
