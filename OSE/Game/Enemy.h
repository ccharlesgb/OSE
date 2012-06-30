#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/EntityList.h"
#include "../Engine/Text.h"

class Enemy : public BasePhysics
{
private:
	BaseObject* mTarget;
	double mLastSearch;
	Vector2 mWanderPos;
	double mLastWander;
public:
	Enemy(void);
	~Enemy(void);
	void Spawn();
	void Think();
	void Draw();

	void TakeDamage(const DamageInfo &info);
	void PhysicsSimulate(float delta);
	void StartTouch(CollisionInfo *info);
	void OnDelete();
	void UpdateWanderPos();
};

